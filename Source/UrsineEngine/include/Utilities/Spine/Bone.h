#pragma once

namespace ursine
{
    namespace spine
    {
        class Skeleton;
        struct BoneData;

        class Bone
        {
            BoneData *data;

            Skeleton *skeleton;

            Bone *parent;

            float x;
            float y;

            float rotation;
            float rotation_ik;

            float scale_x;
            float scale_y;

            int flip_x;
            int flip_y;

            float m00, m01, world_x; /* a b x */
            float m10, m11, world_y; /* c d y */

            float world_rotation;

            float world_scale_x;
            float world_scale_y;

            int world_flip_x;
            int world_flip_y;

            static int y_down;

        public:
            Bone(BoneData *data, Skeleton *skeleton, Bone *parent);
            ~Bone(void);

            const BoneData * GetData(void) const;

            Skeleton * GetSkeleton(void) const;

            Bone * GetParent(void) const;

            float GetX(void) const;
            void SetX(float x);
            float GetY(void) const;
            void SetY(float y);

            float GetRotation(void) const;
            void SetRotation(float rotation);
            float GetRotationIK(void) const;
            void SetRotationIK(float rotation_ik);

            float GetScaleX(void) const;
            void SetScaleX(float x);
            float GetScaleY(void) const;
            void SetScaleY(float y);

            int GetFlipX(void) const;
            void SetFlipX(int flip_x);
            int GetFlipY(void) const;
            void SetFlipY(int flip_y);

            float GetM00(void) const;
            float GetM01(void) const;
            float GetM10(void) const;
            float GetM11(void) const;

            float GetWorldX(void) const;
            float GetWorldY(void) const;

            float GetWorldRotation(void) const;

            float GetWorldScaleX(void) const;
            float GetWorldScaleY(void) const;

            int GetWorldFlipX(void) const;
            int GetWorldFlipY(void) const;

            static void SetYDown(int y_down);
            static int IsYDown(void);

            void SetToSetupPose(void);

            void UpdateWorldTransform(void);

            void WorldToLocal(float worldX, float worldY, float *localX, float *localY);
            void LocalToWorld(float localX, float localY, float *worldX, float *worldY);
        };

    }
}
