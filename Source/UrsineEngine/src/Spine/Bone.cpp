#include "UrsinePrecompiled.h"

#include "Spine/spine.h"

namespace Ursine
{
    namespace Spine
    {
        URSINE_TODO("verify it's true by default");
        int Bone::y_down = 1;

        Bone::Bone(BoneData *data, Skeleton *skeleton, Bone *parent)
            : data(data)
            , skeleton(skeleton)
            , parent(parent)
            , x(0)
            , y(0)
            , rotation(0)
            , rotation_ik(0)
            , scale_x(0)
            , scale_y(0)
            , flip_x(0)
            , flip_y(0)
            , m00(0)
            , m01(0)
            , world_x(0)
            , m10(0)
            , m11(0)
            , world_y(0)
            , world_rotation(0)
            , world_scale_x(0)
            , world_scale_y(0)
            , world_flip_x(0)
            , world_flip_y(0)
        {
            SetToSetupPose();
        }

        Bone::~Bone(void) { }

        const BoneData * Bone::GetData(void) const
        {
            return data;
        }

        Skeleton * Bone::GetSkeleton(void) const
        {
            return skeleton;
        }

        Bone * Bone::GetParent(void) const
        {
            return parent;
        }

        float Bone::GetX(void) const
        {
            return x;
        }

        void Bone::SetX(float x)
        {
            this->x = x;
        }

        float Bone::GetY(void) const
        {
            return y;
        }

        void Bone::SetY(float y)
        {
            this->y = y;
        }

        float Bone::GetRotation(void) const
        {
            return rotation;
        }

        void Bone::SetRotation(float rotation)
        {
            this->rotation = rotation;
        }

        float Bone::GetRotationIK(void) const
        {
            return rotation_ik;
        }

        void Bone::SetRotationIK(float rotation_ik)
        {
            this->rotation_ik = rotation_ik;
        }

        float Bone::GetScaleX(void) const
        {
            return scale_x;
        }

        void Bone::SetScaleX(float scale_x)
        {
            this->scale_x = scale_x;
        }

        float Bone::GetScaleY(void) const
        {
            return scale_y;
        }

        void Bone::SetScaleY(float scale_y)
        {
            this->scale_y = scale_y;
        }

        int Bone::GetFlipX(void) const
        {
            return flip_x;
        }

        void Bone::SetFlipX(int flip_x)
        {
            this->flip_x = flip_x;
        }

        int Bone::GetFlipY(void) const
        {
            return flip_y;
        }

        void Bone::SetFlipY(int flip_y)
        {
            this->flip_y = flip_y;
        }

        float Bone::GetM00(void) const
        {
            return m00;
        }

        float Bone::GetM01(void) const
        {
            return m01;
        }

        float Bone::GetM10(void) const
        {
            return m10;
        }

        float Bone::GetM11(void) const
        {
            return m11;
        }

        float Bone::GetWorldX(void) const
        {
            return world_x;
        }

        float Bone::GetWorldY(void) const
        {
            return world_y;
        }

        float Bone::GetWorldRotation(void) const
        {
            return world_rotation;
        }

        float Bone::GetWorldScaleX(void) const
        {
            return world_scale_x;
        }

        float Bone::GetWorldScaleY(void) const
        {
            return world_scale_y;
        }

        int Bone::GetWorldFlipX(void) const
        {
            return world_flip_x;
        }

        int Bone::GetWorldFlipY(void) const
        {
            return world_flip_y;
        }

        void Bone::SetYDown(int y_down)
        {
            Bone::y_down = y_down;
        }

        int Bone::IsYDown(void)
        {
            return y_down;
        }

        void Bone::SetToSetupPose(void)
        {
            x = data->x;
            y = data->y;
            rotation = data->rotation;
            rotation_ik = rotation;
            scale_x = data->scale_x;
            scale_y = data->scale_y;
            flip_x = data->flip_x;
            flip_y = data->flip_y;
        }

        void Bone::UpdateWorldTransform(void)
        {
            float radians, cosine, sine;
            if (parent)
            {
                world_x = x * parent->m00 + y * parent->m01 + parent->world_x;
                world_y = x * parent->m10 + y * parent->m11 + parent->world_y;
                if (data->inherit_scale)
                {
                    world_scale_x = parent->world_scale_x * scale_x;
                    world_scale_y = parent->world_scale_y * scale_y;
                }
                else
                {
                    world_scale_x = scale_x;
                    world_scale_y = scale_y;
                }
                world_rotation =
                    data->inherit_rotation ? parent->world_rotation + rotation_ik : rotation_ik;
                world_flip_x = parent->world_flip_x ^ flip_x;
                world_flip_y = parent->world_flip_y ^ flip_y;
            }
            else
            {
                int skeletonFlipX = skeleton->GetFlipX(), skeletonFlipY = skeleton->GetFlipY();
                world_x = skeleton->GetFlipX() ? -x : x;
                world_y = skeleton->GetFlipY() != (int)y_down ? -y : y;
                world_scale_x = scale_x;
                world_scale_y = scale_y;
                world_rotation = rotation_ik;
                world_flip_x = skeletonFlipX ^ flip_x;
                world_flip_y = skeletonFlipY ^ flip_y;
            }
            radians = Math::DegreesToRadians(world_rotation);
            cosine = cos(radians);
            sine = sin(radians);
            if (world_flip_x)
            {
                m00 = -cosine * world_scale_x;
                m01 = sine * world_scale_y;
            }
            else
            {
                m00 = cosine * world_scale_x;
                m01 = -sine * world_scale_y;
            }
            if (world_flip_y != y_down)
            {
                m10 = -sine * world_scale_x;
                m11 = -cosine * world_scale_y;
            }
            else
            {
                m10 = sine * world_scale_x;
                m11 = cosine * world_scale_y;
            }
        }

        void Bone::WorldToLocal(float worldX, float worldY, float *localX, float *localY)
        {
            float invDet;
            float dx = worldX - world_x, dy = worldY - world_y;
            float m_00 = m00, m_11 = m11;

            if (world_flip_x != (world_flip_y != y_down))
            {
                m_00 *= -1;
                m_11 *= -1;
            }

            invDet = 1 / (m_00 * m_11 - m01 * m10);
            *localX = (dx * m_00 * invDet - dy * m01 * invDet);
            *localY = (dy * m_11 * invDet - dx * m10 * invDet);
        }

        void Bone::LocalToWorld(float localX, float localY, float *worldX, float *worldY)
        {
            *worldX = localX * m00 + localY * m01 + world_x;
            *worldY = localX * m10 + localY * m11 + world_y;
        }
    }
}
