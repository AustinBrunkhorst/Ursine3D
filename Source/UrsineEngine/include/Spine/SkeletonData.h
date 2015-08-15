#pragma once

#include "Spine/BoneData.h"
#include "Spine/SlotData.h"
#include "Spine/Skin.h"
#include "Spine/EventData.h"
#include "Spine/SpineAnimation.h"
#include "Spine/IkConstraintData.h"

namespace Ursine
{
    namespace Spine
    {
        struct SkeletonData
        {
            std::string version;
            std::string hash;

            float width;
            float height;

            int bone_count;
            BoneData **bones;

            int slot_count;
            SlotData **slots;

            int skin_count;
            Skin **skins;
            Skin *default_skin;

            int event_count;
            EventData **events;

            int animation_count;
            SpineAnimation **animations;

            int ik_constraint_count;
            IkConstraintData **ik_constraints;

            SkeletonData(void);
            ~SkeletonData(void);

            BoneData *FindBone(const char *boneName) const;
            int FindBoneIndex(const char *boneName) const;

            SlotData *FindSlot(const char *slotName) const;
            int FindSlotIndex(const char *slotName) const;

            Skin *FindSkin(const char *skinName) const;

            EventData *FindEvent(const char *eventName) const;

            SpineAnimation *FindAnimation(const char *animationName) const;

            IkConstraintData *FindIkConstraint(const char *ikConstraintName) const;
        };
    }
}
