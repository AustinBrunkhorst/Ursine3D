#pragma once

#include "Spine/SkeletonData.h"
#include "Spine/Slot.h"
#include "Spine/Skin.h"
#include "Spine/IkConstraint.h"

namespace ursine
{
    namespace spine
    {
        class Skeleton
        {
            int bone_cache_count;
            int *bone_cache_counts;
            Bone ***bone_cache;

            SkeletonData * const data;

            int bone_count;
            Bone **bones;
            Bone * const root;

            int slot_count;
            Slot **slots;
            Slot **draw_order;

            int ik_constraint_count;
            IkConstraint **ik_constraints;

            Skin * const skin;

            float r;
            float g;
            float b;
            float a;

            float time;

            int flip_x;
            int flip_y;

            float x;
            float y;

            void setBonesToSetupPose(void);
            void setSlotsToSetupPose(void);

        public:
            Skeleton(SkeletonData *data);
            ~Skeleton(void);

            void UpdateCache(void);
            void UpdateWorldTransform(void);

            void SetToSetupPose(void);

            Bone *FindBone(const char *boneName) const;

            int FindBoneIndex(const char *boneName) const;

            Slot *FindSlot(const char *slotName) const;

            int FindSlotIndex(const char *slotName) const;

            void SetSkin(Skin *new_skin);

            int SetSkin(const char *skinName);

            Attachment *GetAttachmentForSlotName(const char *slotName, const char *attachmentName);
            Attachment *GetAttachmentForSlotIndex(int slotIndex, const char *attachmentName);

            int SetAttachment(const char *slotName, const char *attachmentName);

            IkConstraint *FindIkConstraint(const char *ikConstraintName);

            void Update(float deltaTime);

            SkeletonData *GetData(void) const;

            int GetSlotCount(void) const;
            Slot **GetSlots(void) const;
            Slot **GetDrawOrder(void) const;

            Bone **GetBones(void) const;

            IkConstraint **GetIKConstraints(void) const;

            Skin *GetSkin(void) const;

            float GetR(void) const;
            float GetG(void) const;
            float GetB(void) const;
            float GetA(void) const;

            float GetTime(void) const;

            int GetFlipX(void) const;
            int GetFlipY(void) const;

            float GetX(void) const;
            void SetX(float x);
            float GetY(void) const;
            void SetY(float y);
        };
    }
}
