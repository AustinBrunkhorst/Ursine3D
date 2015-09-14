#include "UrsinePrecompiled.h"

#include "Spine/Skeleton.h"

#include <string.h>

namespace ursine
{
    namespace spine
    {
        Skeleton::Skeleton(SkeletonData *Data)
            : bone_cache_count(0)
            , bone_cache_counts(nullptr)
            , bone_cache(nullptr)
            , data(Data)
            , bone_count(Data->bone_count)
            , bones(new Bone*[bone_count])
            , root(bones[0])
            , slot_count(0)
            , slots(nullptr)
            , draw_order(nullptr)
            , ik_constraint_count(0)
            , ik_constraints(nullptr)
            , skin(nullptr)
            , r(0)
            , g(0)
            , b(0)
            , a(0)
            , time(0)
            , flip_x(0)
            , flip_y(0)
            , x(0)
            , y(0)
        {
            int i, ii;

            for (i = 0; i < bone_count; ++i)
            {
                BoneData *boneData = data->bones[i];
                Bone *parent = 0;
                if (boneData->parent)
                {
                    /* Find parent bone. */
                    for (ii = 0; ii < bone_count; ++ii)
                    {
                        if (Data->bones[ii] == boneData->parent)
                        {
                            parent = bones[ii];
                            break;
                        }
                    }
                }

                bones[i] = new Bone(boneData, this, parent);
            }

            slot_count = Data->slot_count;
            slots = new Slot*[slot_count];
            for (i = 0; i < slot_count; ++i)
            {
                SlotData *slotData = Data->slots[i];

                /* Find bone for the slotData's boneData. */
                Bone *bone = 0;
                for (ii = 0; ii < bone_count; ++ii)
                {
                    if (Data->bones[ii] == slotData->bone_data)
                    {
                        bone = bones[ii];
                        break;
                    }
                }
                slots[i] = new Slot(slotData, bone);
            }

            draw_order = new Slot*[slot_count];
            memcpy(draw_order, slots, sizeof(Slot*) * slot_count);

            r = 1;
            g = 1;
            b = 1;
            a = 1;

            ik_constraint_count = Data->ik_constraint_count;
            ik_constraints = new IkConstraint*[ik_constraint_count];

            for (i = 0; i < data->ik_constraint_count; ++i)
                ik_constraints[i] = new IkConstraint(data->ik_constraints[i], this);

            UpdateCache();
        }

        Skeleton::~Skeleton(void)
        {
            int i;

            for (i = 0; i < bone_cache_count; ++i)
                delete []bone_cache[i];

            delete []bone_cache;
            delete []bone_cache_counts;

            for (i = 0; i < bone_count; ++i)
                delete bones[i];

            delete []bones;

            for (i = 0; i < slot_count; ++i)
                delete slots[i];

            delete []slots;

            for (i = 0; i < ik_constraint_count; ++i)
                delete ik_constraints[i];

            delete []ik_constraints;

            delete []draw_order;
        }

        void Skeleton::UpdateCache(void)
        {
            int i, ii;

            for (i = 0; i < bone_cache_count; ++i)
                delete []bone_cache[i];

            delete []bone_cache;
            delete []bone_cache_counts;

            bone_cache_count = ik_constraint_count + 1;
            bone_cache = new Bone**[bone_cache_count];
            
            bone_cache_counts = new int[bone_cache_count]();

            /* Compute array sizes. */
            for (i = 0; i < bone_count; ++i)
            {
                Bone *current = bones[i];
                do
                {
                    for (ii = 0; ii < ik_constraint_count; ++ii)
                    {
                        IkConstraint *ikConstraint = ik_constraints[ii];
                        Bone *parent = ikConstraint->bones[0];
                        Bone *child = ikConstraint->bones[ikConstraint->bone_count - 1];
                        while (1)
                        {
                            if (current == child)
                            {
                                bone_cache_counts[ii]++;
                                bone_cache_counts[ii + 1]++;
                                goto outer1;
                            }
                            if (child == parent)
                                break;
                            child = child->GetParent();
                        }
                    }
                    current = current->GetParent();
                } while (current);
                bone_cache_counts[0]++;
            outer1: {}
            }

            for (i = 0; i < bone_cache_count; ++i)
                bone_cache[i] = new Bone*[bone_cache_counts[i]];

            memset(bone_cache_counts, 0, bone_cache_count * sizeof(int));

            /* Populate arrays. */
            for (i = 0; i < bone_count; ++i)
            {
                Bone *bone = bones[i];
                Bone *current = bone;
                do
                {
                    for (ii = 0; ii < ik_constraint_count; ++ii)
                    {
                        IkConstraint *ikConstraint = ik_constraints[ii];
                        Bone *parent = ikConstraint->bones[0];
                        Bone *child = ikConstraint->bones[ikConstraint->bone_count - 1];
                        while (1)
                        {
                            if (current == child)
                            {
                                bone_cache[ii][bone_cache_counts[ii]++] = bone;
                                bone_cache[ii + 1][bone_cache_counts[ii + 1]++] = bone;
                                goto outer2;
                            }
                            if (child == parent)
                                break;
                            child = child->GetParent();
                        }
                    }
                    current = current->GetParent();
                } while (current);
                bone_cache[0][bone_cache_counts[0]++] = bone;
            outer2: {}
            }
        }

        void Skeleton::UpdateWorldTransform(void)
        {
            int i, ii, nn, last;

            for (i = 0; i < bone_count; ++i)
                bones[i]->SetRotationIK(bones[i]->GetRotation());

            i = 0;
            last = bone_cache_count - 1;

            while (true)
            {
                for (ii = 0, nn = bone_cache_counts[i]; ii < nn; ++ii)
                    bone_cache[i][ii]->UpdateWorldTransform();

                if (i == last)
                    break;

                ik_constraints[i]->Apply();

                i++;
            }
        }

        void Skeleton::SetToSetupPose(void)
        {
            setBonesToSetupPose();
            setSlotsToSetupPose();
        }

        Bone *Skeleton::FindBone(const char *bone_name) const
        {
            int i;

            for (i = 0; i < bone_count; ++i)
                if (strcmp(data->bones[i]->name.c_str(), bone_name) == 0)
                    return bones[i];

            return nullptr;
        }

        int Skeleton::FindBoneIndex(const char *bone_name) const
        {
            int i;

            for (i = 0; i < bone_count; ++i)
                if (strcmp(data->bones[i]->name.c_str(), bone_name) == 0)
                    return i;

            return -1;
        }

        Slot *Skeleton::FindSlot(const char *slot_name) const
        {
            int i;

            for (i = 0; i < slot_count; ++i)
                if (strcmp(data->slots[i]->name.c_str(), slot_name) == 0)
                    return slots[i];

            return nullptr;
        }

        int Skeleton::FindSlotIndex(const char *slot_name) const
        {
            int i;

            for (i = 0; i < slot_count; ++i)
                if (strcmp(data->slots[i]->name.c_str(), slot_name) == 0)
                    return i;

            return -1;
        }

        void Skeleton::SetSkin(Skin *new_skin)
        {
            if (new_skin)
            {
                if (skin)
                    new_skin->AttachAll(this, skin);
                else
                {
                    /* No previous skin, attach setup pose attachments. */
                    int i;
                    for (i = 0; i < slot_count; ++i)
                    {
                        Slot *slot = slots[i];

                        if (slot->data->attachment_name.c_str())
                        {
                            Attachment *attachment = new_skin->GetAttachment(i, slot->data->attachment_name.c_str());

                            if (attachment)
                                slot->SetAttachment(attachment);
                        }
                    }
                }
            }

            // cast away the const-ness
            (*(Skin**)&skin) = new_skin;
        }

        int Skeleton::SetSkin(const char *skin_name)
        {
            Skin *skin;

            if (!skin_name)
            {
                SetSkin(static_cast<Skin*>(nullptr));

                return 1;
            }

            skin = data->FindSkin(skin_name);

            if (!skin)
                return 0;

            SetSkin(skin);

            return 1;
        }

        Attachment *Skeleton::GetAttachmentForSlotName(const char *slot_name, const char *attachment_name)
        {
            int slot_index = data->FindSlotIndex(slot_name);

            return GetAttachmentForSlotIndex(slot_index, attachment_name);
        }

        Attachment *Skeleton::GetAttachmentForSlotIndex(int slot_index, const char *attachment_name)
        {
            if (slot_index == -1)
                return nullptr;

            if (skin)
            {
                Attachment *attachment = skin->GetAttachment(slot_index, attachment_name);

                if (attachment)
                    return attachment;
            }

            if (data->default_skin)
            {
                Attachment *attachment = data->default_skin->GetAttachment(slot_index, attachment_name);

                if (attachment)
                    return attachment;
            }

            return nullptr;
        }

        int Skeleton::SetAttachment(const char *slot_name, const char *attachment_name)
        {
            int i;

            for (i = 0; i < slot_count; ++i)
            {
                Slot *slot = slots[i];

                if (strcmp(slot->data->name.c_str(), slot_name) == 0)
                {
                    if (!attachment_name)
                        slot->SetAttachment(nullptr);
                    else
                    {
                        Attachment *attachment = GetAttachmentForSlotIndex(i, attachment_name);

                        if (!attachment)
                            return 0;

                        slot->SetAttachment(attachment);
                    }

                    return 1;
                }
            }

            return 0;
        }


        IkConstraint *Skeleton::FindIkConstraint(const char *constraint_name)
        {
            int i;

            for (i = 0; i < ik_constraint_count; ++i)
                if (strcmp(ik_constraints[i]->data->name.c_str(), constraint_name) == 0)
                    return ik_constraints[i];

            return nullptr;
        }

        void Skeleton::Update(float dt)
        {
            time += dt;
        }

        SkeletonData *Skeleton::GetData(void) const
        {
            return data;
        }

        int Skeleton::GetSlotCount(void) const
        {
            return slot_count;
        }

        Slot **Skeleton::GetSlots(void) const
        {
            return slots;
        }

        Slot **Skeleton::GetDrawOrder(void) const
        {
            return draw_order;
        }

        Bone **Skeleton::GetBones(void) const
        {
            return bones;
        }

        IkConstraint **Skeleton::GetIKConstraints(void) const
        {
            return ik_constraints;
        }

        Skin *Skeleton::GetSkin(void) const
        {
            return skin;
        }

        float Skeleton::GetR(void) const
        {
            return r;
        }

        float Skeleton::GetG(void) const
        {
            return g;
        }

        float Skeleton::GetB(void) const
        {
            return b;
        }

        float Skeleton::GetA(void) const
        {
            return a;
        }

        float Skeleton::GetTime(void) const
        {
            return time;
        }

        int Skeleton::GetFlipX(void) const
        {
            return flip_x;
        }

        int Skeleton::GetFlipY(void) const
        {
            return flip_y;
        }

        float Skeleton::GetX(void) const
        {
            return x;
        }

        void Skeleton::SetX(float x)
        {
            this->x = x;
        }

        float Skeleton::GetY(void) const
        {
            return y;
        }

        void Skeleton::SetY(float y)
        {
            this->y = y;
        }

        void Skeleton::setBonesToSetupPose(void)
        {
            int i;

            for (i = 0; i < bone_count; ++i)
                bones[i]->SetToSetupPose();

            for (i = 0; i < ik_constraint_count; ++i)
            {
                IkConstraint *ikConstraint = ik_constraints[i];

                ikConstraint->bend_direction = ikConstraint->data->bend_direction;
                ikConstraint->mix = ikConstraint->data->mix;
            }
        }

        void Skeleton::setSlotsToSetupPose(void)
        {
            int i;

            memcpy(draw_order, slots, slot_count * sizeof(Slot*));

            for (i = 0; i < slot_count; ++i)
                slots[i]->SetToSetupPose();
        }
    }
}
