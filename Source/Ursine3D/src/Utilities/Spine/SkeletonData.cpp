#include "UrsinePrecompiled.h"

#include "Spine/Spine.h"

#include <string.h>

namespace ursine
{
    namespace spine
    {
        SkeletonData::SkeletonData(void)
            : width(0)
            , height(0)
            , bone_count(0)
            , bones(nullptr)
            , slot_count(0)
            , slots(nullptr)
            , skin_count(0)
            , skins(nullptr)
            , default_skin(nullptr)
            , event_count(0)
            , events(nullptr)
            , animation_count(0)
            , animations(nullptr)
            , ik_constraint_count(0)
            , ik_constraints(nullptr)
        {

        }

        SkeletonData::~SkeletonData(void)
        {
            int i;

            for (i = 0; i < bone_count; ++i)
                delete bones[i];

            delete []bones;

            for (i = 0; i < slot_count; ++i)
                delete slots[i];

            delete []slots;

            for (i = 0; i < skin_count; ++i)
                delete skins[i];

            delete []skins;

            for (i = 0; i < event_count; ++i)
                delete events[i];

            delete []events;

            for (i = 0; i < animation_count; ++i)
                delete animations[i];

            delete []animations;

            for (i = 0; i < ik_constraint_count; ++i)
                delete ik_constraints[i];

            delete []ik_constraints;
        }

        BoneData *SkeletonData::FindBone(const char *boneName) const
        {
            int i;

            for (i = 0; i < bone_count; ++i)
                if (strcmp(bones[i]->name.c_str(), boneName) == 0)
                    return bones[i];

            return nullptr;
        }

        int SkeletonData::FindBoneIndex(const char *boneName) const
        {
            int i;

            for (i = 0; i < bone_count; ++i)
                if (strcmp(bones[i]->name.c_str(), boneName) == 0)
                    return i;

            return -1;
        }

        SlotData *SkeletonData::FindSlot(const char *slotName) const
        {
            int i;

            for (i = 0; i < slot_count; ++i)
                if (strcmp(slots[i]->name.c_str(), slotName) == 0)
                    return slots[i];

            return nullptr;
        }

        int SkeletonData::FindSlotIndex(const char *slotName) const
        {
            int i;

            for (i = 0; i < slot_count; ++i)
                if (strcmp(slots[i]->name.c_str(), slotName) == 0)
                    return i;

            return -1;
        }

        Skin *SkeletonData::FindSkin(const char *skinName) const
        {
            int i;

            for (i = 0; i < skin_count; ++i)
                if (strcmp(skins[i]->GetName(), skinName) == 0)
                    return skins[i];

            return nullptr;
        }

        EventData *SkeletonData::FindEvent(const char *eventName) const
        {
            int i;

            for (i = 0; i < event_count; ++i)
                if (strcmp(events[i]->name.c_str(), eventName) == 0)
                    return events[i];

            return nullptr;
        }

        SpineAnimation *SkeletonData::FindAnimation(const char *animationName) const
        {
            int i;

            for (i = 0; i < animation_count; ++i)
                if (strcmp(animations[i]->GetName(), animationName) == 0)
                    return animations[i];

            return nullptr;
        }

        IkConstraintData *SkeletonData::FindIkConstraint(const char *ikConstraintName) const
        {
            int i;
            for (i = 0; i < ik_constraint_count; ++i)
                if (strcmp(ik_constraints[i]->name.c_str(), ikConstraintName) == 0)
                    return ik_constraints[i];

            return nullptr;
        }
    }
}
