#include "UrsinePrecompiled.h"

#include "Spine/SkeletonJson.h"
#include "Spine/SpineJson.h"
#include "Spine/spine.h"

#include <stdio.h>

namespace Ursine
{
    namespace Spine
    {
        static float toColor(const char *value, int index)
        {
            char digits[3];
            char *error;
            int color;

            if (strlen(value) != 8)
                return -1;

            value += index * 2;

            digits[0] = *value;
            digits[1] = *(value + 1);
            digits[2] = '\0';

            color = (int)strtoul(digits, &error, 16);

            if (*error != 0)
                return -1;

            return color / (float)255;
        }

        static void readCurve(CurveTimeline *timeline, int frameIndex, SpineJson *frame)
        {
            SpineJson *curve = Json_getItem(frame, "curve");

            if (!curve)
                return;

            if (curve->type == Json_String && strcmp(curve->valueString, "stepped") == 0)
                timeline->SetStepped(frameIndex);

            else if (curve->type == Json_Array)
            {
                SpineJson *child0 = curve->child;
                SpineJson *child1 = child0->next;
                SpineJson *child2 = child1->next;
                SpineJson *child3 = child2->next;

                timeline->SetCurve(frameIndex, child0->valueFloat, child1->valueFloat, child2->valueFloat, child3->valueFloat);
            }
        }

        SkeletonJson::SkeletonJson(Atlas *atlas)
            : owns_loader(true)
            , scale(1)
            , attachment_loader(new AtlasAttachmentLoader(atlas)) {}

        SkeletonJson::SkeletonJson(AttachmentLoader *attachmentLoader)
            : owns_loader(false)
            , scale(1)
            , attachment_loader(attachmentLoader) {}

        SkeletonJson::~SkeletonJson(void)
        {
            if (owns_loader)
                delete attachment_loader;
        }

        SkeletonData *SkeletonJson::ReadData(const char *json)
        {
            int i;
            int ii;

            SkeletonData *skeletonData;

            SpineJson *root;
            SpineJson *skeleton;
            SpineJson *bones;
            SpineJson *boneMap;
            SpineJson *ik;
            SpineJson *slots;
            SpineJson *skins;
            SpineJson *animations;
            SpineJson *events;

            root = Json_create(json);

            UAssert(root, "Invalid skeleton JSON: %s", Json_getError());

            skeletonData = new SkeletonData();

            skeleton = Json_getItem(root, "skeleton");

            if (skeleton)
            {
                skeletonData->hash = Json_getString(skeleton, "hash", 0);
                skeletonData->version = Json_getString(skeleton, "spine", 0);

                skeletonData->width = Json_getFloat(skeleton, "width", 0);
                skeletonData->height = Json_getFloat(skeleton, "height", 0);
            }

            /* Bones. */
            bones = Json_getItem(root, "bones");

            skeletonData->bones = new BoneData* [bones->size];

            for (boneMap = bones->child, i = 0; boneMap; boneMap = boneMap->next, ++i)
            {
                BoneData *boneData;

                BoneData *parent = nullptr;

                const char *parentName = Json_getString(boneMap, "parent", nullptr);

                if (parentName)
                {
                    parent = skeletonData->FindBone(parentName);

                    UAssert(parent, "Parent bone not found: %s", parentName);
                }

                boneData = new BoneData(Json_getString(boneMap, "name", nullptr), parent);

                boneData->length = Json_getFloat(boneMap, "length", 0) * scale;
                boneData->x = Json_getFloat(boneMap, "x", 0) * scale;
                boneData->y = Json_getFloat(boneMap, "y", 0) * scale;
                boneData->rotation = Json_getFloat(boneMap, "rotation", 0);
                boneData->scale_x = Json_getFloat(boneMap, "scaleX", 1);
                boneData->scale_y = Json_getFloat(boneMap, "scaleY", 1);
                boneData->inherit_scale = Json_getInt(boneMap, "inheritScale", 1) ? true : false;
                boneData->inherit_rotation = Json_getInt(boneMap, "inheritRotation", 1) ? true : false;
                boneData->flip_x = Json_getInt(boneMap, "flipX", 0) ? true : false;
                boneData->flip_y = Json_getInt(boneMap, "flipY", 0) ? true : false;

                skeletonData->bones[i] = boneData;
                skeletonData->bone_count++;
            }

            /* IK constraints. */
            ik = Json_getItem(root, "ik");

            if (ik)
            {
                SpineJson *ikMap;

                skeletonData->ik_constraint_count = ik->size;
                skeletonData->ik_constraints = new IkConstraintData* [ik->size];

                for (ikMap = ik->child, i = 0; ikMap; ikMap = ikMap->next, ++i)
                {
                    const char *targetName;

                    IkConstraintData *ikConstraintData = new IkConstraintData(Json_getString(ikMap, "name", nullptr));

                    boneMap = Json_getItem(ikMap, "bones");

                    ikConstraintData->bone_count = boneMap->size;
                    ikConstraintData->bones = new BoneData* [boneMap->size];

                    for (boneMap = boneMap->child, ii = 0; boneMap; boneMap = boneMap->next, ++ii)
                    {
                        ikConstraintData->bones[ii] = skeletonData->FindBone(boneMap->valueString);

                        UAssert(ikConstraintData->bones[ii], "IK bone not found: %s", boneMap->valueString);
                    }

                    targetName = Json_getString(ikMap, "target", nullptr);

                    ikConstraintData->target = skeletonData->FindBone(targetName);

                    UAssert(ikConstraintData->target, "Target bone not found: %s", boneMap->name);

                    ikConstraintData->bend_direction = Json_getInt(ikMap, "bendPositive", 1) ? 1 : -1;
                    ikConstraintData->mix = Json_getFloat(ikMap, "mix", 1);

                    skeletonData->ik_constraints[i] = ikConstraintData;
                }
            }

            /* Slots. */
            slots = Json_getItem(root, "slots");

            if (slots)
            {
                SpineJson *slotMap;

                skeletonData->slot_count = slots->size;
                skeletonData->slots = new SlotData* [slots->size];

                for (slotMap = slots->child, i = 0; slotMap; slotMap = slotMap->next, ++i)
                {
                    SlotData *slotData;
                    const char *color;
                    SpineJson *attachmentItem;

                    const char *boneName = Json_getString(slotMap, "bone", nullptr);

                    BoneData *boneData = skeletonData->FindBone(boneName);

                    UAssert(boneData, "Slot bone not found: %s", boneName);

                    slotData = new SlotData(Json_getString(slotMap, "name", nullptr), boneData);

                    color = Json_getString(slotMap, "color", nullptr);

                    if (color)
                    {
                        slotData->r = toColor(color, 0);
                        slotData->g = toColor(color, 1);
                        slotData->b = toColor(color, 2);
                        slotData->a = toColor(color, 3);
                    }

                    attachmentItem = Json_getItem(slotMap, "attachment");

                    if (attachmentItem)
                        slotData->SetAttachmentName(attachmentItem->valueString);

                    slotData->additive_blending = Json_getInt(slotMap, "additive", 0) ? true : false;

                    skeletonData->slots[i] = slotData;
                }
            }

            /* Skins. */
            skins = Json_getItem(root, "skins");

            if (skins)
            {
                SpineJson *slotMap;

                skeletonData->skin_count = skins->size;
                skeletonData->skins = new Skin* [skins->size];

                for (slotMap = skins->child, i = 0; slotMap; slotMap = slotMap->next, ++i)
                {
                    SpineJson *attachmentsMap;

                    Skin *skin = new Skin(slotMap->name);

                    skeletonData->skins[i] = skin;

                    if (strcmp(slotMap->name, "default") == 0)
                        skeletonData->default_skin = skin;

                    for (attachmentsMap = slotMap->child; attachmentsMap; attachmentsMap = attachmentsMap->next)
                    {
                        int slotIndex = skeletonData->FindSlotIndex(attachmentsMap->name);

                        SpineJson *attachmentMap;

                        for (attachmentMap = attachmentsMap->child; attachmentMap; attachmentMap = attachmentMap->next)
                        {
                            Attachment *attachment;

                            const char *skinAttachmentName = attachmentMap->name;
                            const char *attachmentName = Json_getString(attachmentMap, "name", skinAttachmentName);
                            const char *path = Json_getString(attachmentMap, "path", attachmentName);
                            const char *color;
                            int i;

                            SpineJson *entry;

                            const char *typeString = Json_getString(attachmentMap, "type", "region");

                            AttachmentType type;

                            if (strcmp(typeString, "region") == 0)
                                type = SP_ATTACHMENT_REGION;
                            else if (strcmp(typeString, "mesh") == 0)
                                type = SP_ATTACHMENT_MESH;
                            else if (strcmp(typeString, "skinnedmesh") == 0)
                                type = SP_ATTACHMENT_SKINNED_MESH;
                            else if (strcmp(typeString, "boundingbox") == 0)
                                type = SP_ATTACHMENT_BOUNDING_BOX;
                            else
                            {
                                UError("Unknown attachment type: %s", typeString);
                            }

                            attachment = attachment_loader->NewAttachment(skin, type, attachmentName, path);

                            switch (attachment->type)
                            {
                            case SP_ATTACHMENT_REGION:
                            {
                                RegionAttachment *region = static_cast<RegionAttachment*>(attachment);

                                if (path)
                                    region->path = path;

                                region->x = Json_getFloat(attachmentMap, "x", 0) * scale;
                                region->y = Json_getFloat(attachmentMap, "y", 0) * scale;
                                region->scale_x = Json_getFloat(attachmentMap, "scaleX", 1);
                                region->scale_y = Json_getFloat(attachmentMap, "scaleY", 1);
                                region->rotation = Json_getFloat(attachmentMap, "rotation", 0);
                                region->width = Json_getFloat(attachmentMap, "width", 32) * scale;
                                region->height = Json_getFloat(attachmentMap, "height", 32) * scale;

                                color = Json_getString(attachmentMap, "color", nullptr);

                                if (color)
                                {
                                    region->r = toColor(color, 0);
                                    region->g = toColor(color, 1);
                                    region->b = toColor(color, 2);
                                    region->a = toColor(color, 3);
                                }

                                region->UpdateOffset();

                                break;
                            }
                            case SP_ATTACHMENT_MESH:
                            {
                                MeshAttachment *mesh = static_cast<MeshAttachment*>(attachment);

                                mesh->path = path;

                                entry = Json_getItem(attachmentMap, "vertices");

                                mesh->vertex_count = entry->size;

                                mesh->vertices = new float [entry->size];

                                for (entry = entry->child, i = 0; entry; entry = entry->next, ++i)
                                    mesh->vertices[i] = entry->valueFloat * scale;

                                entry = Json_getItem(attachmentMap, "triangles");

                                mesh->triangle_count = entry->size;

                                mesh->triangles = new int [entry->size];

                                for (entry = entry->child, i = 0; entry; entry = entry->next, ++i)
                                    mesh->triangles[i] = entry->valueInt;

                                entry = Json_getItem(attachmentMap, "uvs");

                                mesh->region_uvs = new float [entry->size];

                                for (entry = entry->child, i = 0; entry; entry = entry->next, ++i)
                                    mesh->region_uvs[i] = entry->valueFloat;

                                mesh->UpdateUVs();

                                color = Json_getString(attachmentMap, "color", nullptr);

                                if (color)
                                {
                                    mesh->r = toColor(color, 0);
                                    mesh->g = toColor(color, 1);
                                    mesh->b = toColor(color, 2);
                                    mesh->a = toColor(color, 3);
                                }

                                mesh->hull_length = Json_getInt(attachmentMap, "hull", 0);

                                entry = Json_getItem(attachmentMap, "edges");

                                if (entry)
                                {
                                    mesh->edge_count = entry->size;
                                    mesh->edges = new int [entry->size];
                                    for (entry = entry->child, i = 0; entry; entry = entry->next, ++i)
                                        mesh->edges[i] = entry->valueInt;
                                }

                                mesh->width = Json_getFloat(attachmentMap, "width", 32) * scale;
                                mesh->height = Json_getFloat(attachmentMap, "height", 32) * scale;

                                break;
                            }
                            case SP_ATTACHMENT_SKINNED_MESH:
                            {
                                SkinnedMeshAttachment *mesh = static_cast<SkinnedMeshAttachment*>(attachment);

                                int verticesCount, b, w, nn;
                                float *vertices;

                                mesh->path = path;

                                entry = Json_getItem(attachmentMap, "uvs");

                                mesh->uv_count = entry->size;
                                mesh->region_uvs = new float [entry->size];

                                for (entry = entry->child, i = 0; entry; entry = entry->next, ++i)
                                    mesh->region_uvs[i] = entry->valueFloat;

                                entry = Json_getItem(attachmentMap, "vertices");

                                verticesCount = entry->size;
                                vertices = new float [entry->size];

                                for (entry = entry->child, i = 0; entry; entry = entry->next, ++i)
                                    vertices[i] = entry->valueFloat;

                                for (i = 0; i < verticesCount;)
                                {
                                    int bonesCount = (int)vertices[i];

                                    mesh->bone_count += bonesCount + 1;
                                    mesh->weight_count += bonesCount * 3;

                                    i += 1 + bonesCount * 4;
                                }

                                mesh->bones = new int [mesh->bone_count];
                                mesh->weights = new float [mesh->weight_count];

                                for (i = 0, b = 0, w = 0; i < verticesCount;)
                                {
                                    int bonesCount = (int)vertices[i++];

                                    mesh->bones[b++] = bonesCount;

                                    for (nn = i + bonesCount * 4; i < nn; i += 4, ++b, w += 3)
                                    {
                                        mesh->bones[b] = (int)vertices[i];
                                        mesh->weights[w] = vertices[i + 1] * scale;
                                        mesh->weights[w + 1] = vertices[i + 2] * scale;
                                        mesh->weights[w + 2] = vertices[i + 3];
                                    }
                                }

                                delete []vertices;

                                entry = Json_getItem(attachmentMap, "triangles");

                                mesh->triangle_count = entry->size;
                                mesh->triangles = new int [entry->size];

                                for (entry = entry->child, i = 0; entry; entry = entry->next, ++i)
                                    mesh->triangles[i] = entry->valueInt;

                                mesh->UpdateUVs();

                                color = Json_getString(attachmentMap, "color", nullptr);

                                if (color)
                                {
                                    mesh->r = toColor(color, 0);
                                    mesh->g = toColor(color, 1);
                                    mesh->b = toColor(color, 2);
                                    mesh->a = toColor(color, 3);
                                }

                                mesh->hull_length = Json_getInt(attachmentMap, "hull", 0);

                                entry = Json_getItem(attachmentMap, "edges");

                                if (entry)
                                {
                                    mesh->edge_count = entry->size;
                                    mesh->edges = new int [entry->size];

                                    for (entry = entry->child, i = 0; entry; entry = entry->next, ++i)
                                        mesh->edges[i] = entry->valueInt;
                                }

                                mesh->width = Json_getFloat(attachmentMap, "width", 32) * scale;
                                mesh->height = Json_getFloat(attachmentMap, "height", 32) * scale;

                                break;
                            }
                            case SP_ATTACHMENT_BOUNDING_BOX:
                            {
                                BoundingBoxAttachment *box = static_cast<BoundingBoxAttachment*>(attachment);

                                entry = Json_getItem(attachmentMap, "vertices");

                                box->vertex_count = entry->size;
                                box->vertices = new float [entry->size];

                                for (entry = entry->child, i = 0; entry; entry = entry->next, ++i)
                                    box->vertices[i] = entry->valueFloat * scale;

                                break;
                            }
                            }

                            skin->AddAttachment(slotIndex, skinAttachmentName, attachment);
                        }
                    }
                }
            }

            /* Events. */
            events = Json_getItem(root, "events");

            if (events)
            {
                SpineJson *eventMap;
                const char *stringValue;

                skeletonData->event_count = events->size;
                skeletonData->events = new EventData* [events->size];

                for (eventMap = events->child, i = 0; eventMap; eventMap = eventMap->next, ++i)
                {
                    EventData *eventData = new EventData(eventMap->name);

                    eventData->int_value = Json_getInt(eventMap, "int", 0);
                    eventData->float_value = Json_getFloat(eventMap, "float", 0);

                    stringValue = Json_getString(eventMap, "string", nullptr);

                    if (stringValue)
                        eventData->string_value = stringValue;

                    skeletonData->events[i] = eventData;
                }
            }

            /* Animations. */
            animations = Json_getItem(root, "animations");

            if (animations)
            {
                SpineJson *animationMap;

                skeletonData->animations = new SpineAnimation* [animations->size];

                for (animationMap = animations->child; animationMap; animationMap = animationMap->next)
                    readAnimation(animationMap, skeletonData);
            }

            Json_dispose(root);

            return skeletonData;
        }

        SkeletonData *SkeletonJson::ReadFile(const char *path)
        {
            auto json = *Ursine::gContentManager->LoadText(path);

            return ReadData(json.c_str());
        }

        SpineAnimation *SkeletonJson::readAnimation(SpineJson *root, SkeletonData *skeletonData)
        {
            int i;
            SpineAnimation *animation;
            SpineJson *frame;
            float duration;
            int timelinesCount = 0;

            SpineJson *bones = Json_getItem(root, "bones");
            SpineJson *slots = Json_getItem(root, "slots");
            SpineJson *ik = Json_getItem(root, "ik");
            SpineJson *ffd = Json_getItem(root, "ffd");
            SpineJson *drawOrder = Json_getItem(root, "drawOrder");
            SpineJson *events = Json_getItem(root, "events");
            SpineJson *flipX = Json_getItem(root, "flipx");
            SpineJson *flipY = Json_getItem(root, "flipy");

            SpineJson *boneMap;
            SpineJson *slotMap;
            SpineJson *ikMap;
            SpineJson *ffdMap;

            if (!drawOrder)
                drawOrder = Json_getItem(root, "draworder");

            for (boneMap = bones ? bones->child : 0; boneMap; boneMap = boneMap->next)
                timelinesCount += boneMap->size;
            for (slotMap = slots ? slots->child : 0; slotMap; slotMap = slotMap->next)
                timelinesCount += slotMap->size;

            timelinesCount += ik ? ik->size : 0;

            for (ffdMap = ffd ? ffd->child : 0; ffdMap; ffdMap = ffdMap->next)
                for (slotMap = ffdMap->child; slotMap; slotMap = slotMap->next)
                    timelinesCount += slotMap->size;

            if (drawOrder)
                ++timelinesCount;
            if (events)
                ++timelinesCount;
            if (flipX)
                ++timelinesCount;
            if (flipY)
                ++timelinesCount;

            animation = new SpineAnimation(root->name, timelinesCount);

            animation->timeline_count = 0;

            skeletonData->animations[skeletonData->animation_count++] = animation;

            /* Slot timelines. */
            for (slotMap = slots ? slots->child : 0; slotMap; slotMap = slotMap->next)
            {
                SpineJson *timelineArray;

                int slotIndex = skeletonData->FindSlotIndex(slotMap->name);

                UAssert(slotIndex != -1, "Slot not found: %s", slotMap->name);

                for (timelineArray = slotMap->child; timelineArray; timelineArray = timelineArray->next)
                {
                    if (strcmp(timelineArray->name, "color") == 0)
                    {
                        ColorTimeline *timeline = new ColorTimeline(timelineArray->size);

                        timeline->slot_index = slotIndex;

                        for (frame = timelineArray->child, i = 0; frame; frame = frame->next, ++i)
                        {
                            const char *s = Json_getString(frame, "color", nullptr);

                            timeline->SetFrame(i,
                                Json_getFloat(frame, "time", 0),
                                toColor(s, 0),
                                toColor(s, 1),
                                toColor(s, 2),
                                toColor(s, 3));

                            readCurve(timeline, i, frame);
                        }

                        animation->timelines[animation->timeline_count++] = timeline;

                        duration = timeline->frames[timelineArray->size * 5 - 5];

                        if (duration > animation->duration)
                            animation->duration = duration;
                    }
                    else if (strcmp(timelineArray->name, "attachment") == 0)
                    {
                        AttachmentTimeline *timeline = new AttachmentTimeline(timelineArray->size);

                        timeline->slot_index = slotIndex;

                        for (frame = timelineArray->child, i = 0; frame; frame = frame->next, ++i)
                        {
                            SpineJson *name = Json_getItem(frame, "name");

                            timeline->SetFrame(i, Json_getFloat(frame, "time", 0),
                                name->type == Json_NULL ? 0 : name->valueString);
                        }

                        animation->timelines[animation->timeline_count++] = timeline;

                        duration = timeline->frames[timelineArray->size - 1];

                        if (duration > animation->duration)
                            animation->duration = duration;
                    }
                    else
                    {
                        UError("Invalid timeline type for a slot: %s", timelineArray->name);
                    }
                }
            }

            /* Bone timelines. */
            for (boneMap = bones ? bones->child : 0; boneMap; boneMap = boneMap->next)
            {
                SpineJson *timelineArray;

                int boneIndex = skeletonData->FindBoneIndex(boneMap->name);

                UAssert(boneIndex != -1, "Bone not found: %s", boneMap->name);

                for (timelineArray = boneMap->child; timelineArray; timelineArray = timelineArray->next)
                {
                    if (strcmp(timelineArray->name, "rotate") == 0)
                    {
                        RotateTimeline *timeline = new RotateTimeline(timelineArray->size);

                        timeline->bone_index = boneIndex;

                        for (frame = timelineArray->child, i = 0; frame; frame = frame->next, ++i)
                        {
                            timeline->SetFrame(i, Json_getFloat(frame, "time", 0), Json_getFloat(frame, "angle", 0));

                            readCurve(timeline, i, frame);
                        }

                        animation->timelines[animation->timeline_count++] = timeline;

                        duration = timeline->frames[timelineArray->size * 2 - 2];

                        if (duration > animation->duration)
                            animation->duration = duration;
                    }
                    else
                    {
                        int isScale = strcmp(timelineArray->name, "scale") == 0;

                        if (isScale || strcmp(timelineArray->name, "translate") == 0)
                        {
                            float scale = isScale ? 1 : this->scale;

                            // this is hacky, thank you Esoteric software
                            TranslateTimeline *timeline;

                            if (isScale)
                            {
                                timeline = reinterpret_cast<TranslateTimeline*>(
                                    new ScaleTimeline(timelineArray->size)
                                    );
                            }
                            else
                            {
                                timeline = new TranslateTimeline(timelineArray->size);
                            }

                            timeline->bone_index = boneIndex;

                            for (frame = timelineArray->child, i = 0; frame; frame = frame->next, ++i)
                            {
                                timeline->SetFrame(i,
                                    Json_getFloat(frame, "time", 0),
                                    Json_getFloat(frame, "x", 0) * scale,
                                    Json_getFloat(frame, "y", 0) * scale);

                                readCurve(timeline, i, frame);
                            }

                            animation->timelines[animation->timeline_count++] = timeline;

                            duration = timeline->frames[timelineArray->size * 3 - 3];

                            if (duration > animation->duration)
                                animation->duration = duration;
                        }
                        else if (strcmp(timelineArray->name, "flipX") == 0 || strcmp(timelineArray->name, "flipY") == 0)
                        {
                            bool x = strcmp(timelineArray->name, "flipX") == 0;

                            const char *field = x ? "x" : "y";

                            FlipTimeline *timeline = new FlipTimeline(timelineArray->size, x);

                            timeline->bone_index = boneIndex;

                            for (frame = timelineArray->child, i = 0; frame; frame = frame->next, ++i)
                                timeline->SetFrame(i, Json_getFloat(frame, "time", 0), Json_getInt(frame, field, 0) ? true : false);

                            animation->timelines[animation->timeline_count++] = timeline;

                            duration = timeline->frames[timelineArray->size * 2 - 2];

                            if (duration > animation->duration)
                                animation->duration = duration;
                        }
                        else
                        {
                            UError("Invalid timeline type for a bone: %s", timelineArray->name);
                        }
                    }
                }
            }

            /* IK timelines. */
            for (ikMap = ik ? ik->child : 0; ikMap; ikMap = ikMap->next)
            {
                IkConstraintData *ikConstraint = skeletonData->FindIkConstraint(ikMap->name);

                IkConstraintTimeline *timeline = new IkConstraintTimeline(ikMap->size);

                for (i = 0; i < skeletonData->ik_constraint_count; ++i)
                {
                    if (ikConstraint == skeletonData->ik_constraints[i])
                    {
                        timeline->constraint_index = i;
                        break;
                    }
                }
                for (frame = ikMap->child, i = 0; frame; frame = frame->next, ++i)
                {
                    timeline->SetFrame(i,
                        Json_getFloat(frame, "time", 0),
                        Json_getFloat(frame, "mix", 0),
                        Json_getInt(frame, "bendPositive", 1) ? 1 : -1);

                    readCurve(timeline, i, frame);
                }

                animation->timelines[animation->timeline_count++] = timeline;

                duration = timeline->frames[ikMap->size * 3 - 3];

                if (duration > animation->duration)
                    animation->duration = duration;
            }

            /* FFD timelines. */
            for (ffdMap = ffd ? ffd->child : 0; ffdMap; ffdMap = ffdMap->next)
            {
                Skin *skin = skeletonData->FindSkin(ffdMap->name);

                for (slotMap = ffdMap->child; slotMap; slotMap = slotMap->next)
                {
                    int slotIndex = skeletonData->FindSlotIndex(slotMap->name);

                    SpineJson *timelineArray;

                    for (timelineArray = slotMap->child; timelineArray; timelineArray = timelineArray->next)
                    {
                        SpineJson *frame;
                        int verticesCount = 0;
                        float *tempVertices;
                        FFDTimeline *timeline;

                        Attachment *attachment = skin->GetAttachment(slotIndex, timelineArray->name);

                        UAssert(attachment,
                            "Attachment not found: %s", timelineArray->name);

                        if (attachment->type == SP_ATTACHMENT_MESH)
                            verticesCount = static_cast<MeshAttachment*>(attachment)->vertex_count;
                        else if (attachment->type == SP_ATTACHMENT_SKINNED_MESH)
                            verticesCount = static_cast<SkinnedMeshAttachment*>(attachment)->weight_count / 3 * 2;

                        timeline = new FFDTimeline(timelineArray->size, verticesCount);

                        timeline->slot_index = slotIndex;
                        timeline->attachment = attachment;

                        tempVertices = new float [verticesCount];

                        for (frame = timelineArray->child, i = 0; frame; frame = frame->next, ++i)
                        {
                            SpineJson *vertices = Json_getItem(frame, "vertices");

                            float *frameVertices;

                            if (!vertices)
                            {
                                if (attachment->type == SP_ATTACHMENT_MESH)
                                    frameVertices = static_cast<MeshAttachment*>(attachment)->vertices;
                                else
                                {
                                    frameVertices = tempVertices;

                                    memset(frameVertices, 0, sizeof(float) * verticesCount);
                                }
                            }
                            else
                            {
                                int v, start = Json_getInt(frame, "offset", 0);

                                SpineJson *vertex;

                                frameVertices = tempVertices;

                                memset(frameVertices, 0, sizeof(float) * start);

                                if (scale == 1)
                                {
                                    for (vertex = vertices->child, v = start; vertex; vertex = vertex->next, ++v)
                                        frameVertices[v] = vertex->valueFloat;
                                }
                                else
                                {
                                    for (vertex = vertices->child, v = start; vertex; vertex = vertex->next, ++v)
                                        frameVertices[v] = vertex->valueFloat * scale;
                                }

                                memset(frameVertices + v, 0, sizeof(float) * (verticesCount - v));

                                if (attachment->type == SP_ATTACHMENT_MESH)
                                {
                                    float *meshVertices = static_cast<MeshAttachment*>(attachment)->vertices;
                                    for (v = 0; v < verticesCount; ++v)
                                        frameVertices[v] += meshVertices[v];
                                }
                            }

                            timeline->SetFrame(i, Json_getFloat(frame, "time", 0), frameVertices);

                            readCurve(timeline, i, frame);
                        }

                        delete []tempVertices;

                        animation->timelines[animation->timeline_count++] = timeline;

                        duration = timeline->frames[timelineArray->size - 1];

                        if (duration > animation->duration)
                            animation->duration = duration;
                    }
                }
            }

            /* Draw order timeline. */
            if (drawOrder)
            {
                DrawOrderTimeline *timeline = new DrawOrderTimeline(drawOrder->size, skeletonData->slot_count);

                for (frame = drawOrder->child, i = 0; frame; frame = frame->next, ++i)
                {
                    int ii;
                    int *drawOrder = 0;

                    SpineJson *offsets = Json_getItem(frame, "offsets");

                    if (offsets)
                    {
                        SpineJson *offsetMap;

                        int *unchanged = new int [skeletonData->slot_count - offsets->size];

                        int originalIndex = 0;
                        int unchangedIndex = 0;

                        drawOrder = new int [skeletonData->slot_count];

                        for (ii = skeletonData->slot_count - 1; ii >= 0; --ii)
                            drawOrder[ii] = -1;

                        for (offsetMap = offsets->child; offsetMap; offsetMap = offsetMap->next)
                        {
                            int slotIndex = skeletonData->FindSlotIndex(Json_getString(offsetMap, "slot", nullptr));

                            UAssert(slotIndex != -1, "Slot not found: %s", Json_getString(offsetMap, "slot", nullptr));

                            /* Collect unchanged items. */
                            while (originalIndex != slotIndex)
                                unchanged[unchangedIndex++] = originalIndex++;

                            /* Set changed items. */
                            drawOrder[originalIndex + Json_getInt(offsetMap, "offset", 0)] = originalIndex;

                            originalIndex++;
                        }
                        /* Collect remaining unchanged items. */
                        while (originalIndex < skeletonData->slot_count)
                            unchanged[unchangedIndex++] = originalIndex++;

                        /* Fill in unchanged items. */
                        for (ii = skeletonData->slot_count - 1; ii >= 0; ii--)
                            if (drawOrder[ii] == -1)
                                drawOrder[ii] = unchanged[--unchangedIndex];

                        delete []unchanged;
                    }

                    timeline->SetFrame(i, Json_getFloat(frame, "time", 0), drawOrder);

                    delete []drawOrder;
                }

                animation->timelines[animation->timeline_count++] = timeline;

                duration = timeline->frames[drawOrder->size - 1];

                if (duration > animation->duration)
                    animation->duration = duration;
            }

            /* Event timeline. */
            if (events)
            {
                SpineJson *frame;

                EventTimeline *timeline = new EventTimeline(events->size);

                for (frame = events->child, i = 0; frame; frame = frame->next, ++i)
                {
                    SpineEvent *event;
                    const char *stringValue;

                    EventData *eventData = skeletonData->FindEvent(Json_getString(frame, "name", nullptr));

                    UAssert(eventData,
                        "Event not found: %s", Json_getString(frame, "name", nullptr));

                    event = new SpineEvent(eventData);

                    event->int_value = Json_getInt(frame, "int", eventData->int_value);
                    event->float_value = Json_getFloat(frame, "float", eventData->float_value);

                    stringValue = Json_getString(frame, "string", eventData->string_value.c_str());

                    if (stringValue)
                        event->string_value = stringValue;

                    timeline->SetFrame(i, Json_getFloat(frame, "time", 0), event);
                }

                animation->timelines[animation->timeline_count++] = timeline;

                duration = timeline->frames[events->size - 1];

                if (duration > animation->duration)
                    animation->duration = duration;
            }

            return animation;
        }

        float SkeletonJson::GetScale(void) const
        {
            return scale;
        }

        void SkeletonJson::SetScale(float scale)
        {
            this->scale = scale;
        }
    }
}