/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TransformComponent.hpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
**
** -------------------------------------------------------------------------*/

#pragma once

#include "Entity.h"

namespace Ursine
{
    namespace ECS
    {
        inline void Transform::SetRootPosition(const Vec3 &position)
        {
            root->SetPosition(position);
        }

        inline void Transform::AddRootPosition(const Vec3 &position)
        {
            root->AddPosition(position);
        }

        inline void Transform::SetPosition(const Vec3 &position)
        {
            this->position = position;

            updateWorldPosition();
        }

        inline void Transform::AddPosition(const Vec3 &position)
        {
            this->position += position;

            updateWorldPosition();
        }

        inline void Transform::SetPositionWC(const Vec3 &position)
        {
            if (parent)
            {
                this->position = position / parent->_world_scale;
            }
            else
            {
                this->position = position;
            }

            updateWorldPosition();
        }

        inline const Vec3 &Transform::PositionWC(void) const
        {
            return _world_position;
        }

        inline const Vec3 &Transform::LocalPosition(void) const
        {
            return position;
        }

        inline void Transform::AddPositionWC(const Vec3 &position)
        {
            if (parent)
            {
                this->position += position / parent->_world_scale;
            }
            else
            {
                this->position += position;
            }

            updateWorldPosition();
        }

        inline void Transform::SetRootRotation(const Vec3 &rotation)
        {
            root->SetRotation(rotation);
        }

        inline void Transform::AddRootRotation(const Vec3 &rotation)
        {
            root->AddRotation(rotation);
        }

        inline void Transform::SetRotation(const Vec3 &rotation)
        {
            this->rotation = rotation;

            updateWorldRotation();
        }

        inline void Transform::AddRotation(const Vec3 &rotation)
        {
            this->rotation += rotation;

            updateWorldRotation();
        }

        inline const Mat3 &Transform::RotationWC(void) const
        {
            return _world_rotation;
        }

        inline const Vec3 &Transform::LocalRotation(void) const
        {
            return rotation;
        }

        inline void Transform::SetRootRotation2D(float radians)
        {
            root->SetRotation2D(radians);
        }

        inline void Transform::AddRootRotation2D(float radians)
        {
            root->AddRotation2D(radians);
        }

        inline void Transform::SetRotation2D(float radians)
        {
            rotation.Z() = radians;

            updateWorldRotation();
        }

        inline void Transform::AddRotation2D(float radians)
        {
            rotation.Z() += radians;

            updateWorldRotation();
        }

        inline float Transform::RootRotation2D(void) const
        {
            return root->LocalRotation2D();
        }

        inline Vec2 Transform::RootRotation2DVec(void) const
        {
            return Vec2(cos(root->LocalRotation2D()),
                           sin(root->LocalRotation2D()));
        }

        inline float Transform::LocalRotation2D(void) const
        {
            return rotation.Z();
        }

        inline Vec2 Transform::LocalRotation2DVec(void) const
        {
            return Vec2(cos(rotation.Z()),
                           sin(rotation.Z()));
        }

        inline void Transform::SetRootScale(const Vec3 &scale)
        {
            root->SetScale(scale);
        }

        inline void Transform::AddRootScale(const Vec3 &scale)
        {
            root->AddScale(scale);
        }

        inline void Transform::SetScale(const Vec3 &scale)
        {
            this->scale = scale;

            updateWorldScale();
        }

        inline void Transform::AddScale(const Vec3 &scale)
        {
            this->scale += scale;

            updateWorldScale();
        }

        inline void Transform::SetScaleWC(const Vec3 &scale)
        {
            if (parent)
            {
                this->scale = scale / parent->_world_scale;
            }
            else
            {
                this->scale = scale;
            }

            updateWorldScale();
        }

        inline void Transform::AddScaleWC(const Vec3 &scale)
        {
            if (parent)
            {
                this->scale += scale / parent->_world_scale;
            }
            else
            {
                this->scale += scale;
            }

            updateWorldScale();
        }

        inline const Vec3 &Transform::ScaleWC(void) const
        {
            return _world_scale;
        }

        inline const Vec3 &Transform::LocalScale(void) const
        {
            return scale;
        }

        inline Vec3 Transform::ToLocal(const Vec3 &point)
        {
            return WorldToLocalMatrix() * Vec3(point.X(), point.Y(), 1.0f);
        }

        inline Vec2 Transform::ToLocal(const Vec2 &point)
        {
            return WorldToLocalMatrix().TransformPoint(point);
        }

        inline Vec3 Transform::ToWorld(const Vec3 &point)
        {
            return Matrix() * point;
        }

        inline Vec2 Transform::ToWorld(const Vec2 &point)
        {
            return Matrix().TransformPoint(point);
        }

        inline bool Transform::Dirty(void) const
        {
            return _dirty;
        }

        inline Transform *Transform::Root(void)
        {
            return root;
        }

        inline Transform *Transform::Parent(void)
        {
            return parent;
        }

        template<class ComponentType, typename... Args>
        void Transform::ApplyChildren(void (ComponentType::*func)(Args...), Args&&... args)
        {
            for (auto &child : _children)
                child->ApplyChildren(func, std::forward<Args>(args)...);

            auto entity = GetOwner();

            if (entity)
            {
                auto component = entity->GetComponent<ComponentType>();

                if (component)
                    (component->*func)(std::forward<Args>(args)...);
            }
        }
    }
}
