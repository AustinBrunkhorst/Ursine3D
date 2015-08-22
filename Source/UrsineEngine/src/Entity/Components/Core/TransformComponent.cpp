/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TransformComponent.cpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
**
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "TransformComponent.h"

namespace Ursine
{
    namespace ECS
    {
        Transform::Transform(void)
            : BaseComponent()
            , _world_scale(1, 1, 1)
            , _world_rotation()
            , scale(1, 1, 1)
            , _local_to_world()
            , _world_to_local()
            , root(this)
            , parent(nullptr)
            , _dirty(true) { }

        Transform::Transform(const Transform &transform)
            : BaseComponent()
        {
            copy(transform);
        }

        void Transform::OnInitialize(void)
        {
            // update all world coordinates
            // since only our local is set
            // after serialization
            updateWorldPosition();
            updateWorldRotation();
            updateWorldScale();
        }

        Transform::~Transform(void)
        {
            if (parent)
            {
                parent->RemoveChild(this);
            }

            RemoveChildren();
        }

        Transform &Transform::operator=(const Transform &transform)
        {
            copy(transform);

            return (*this);
        }

        void Transform::copy(const Transform &other)
        {
            _world_position = other._world_position;
            _world_scale = other._world_scale;
            _world_rotation = other._world_rotation;

            position = other.position;
            rotation = other.rotation;
            scale = other.scale;

            _local_to_world = other._local_to_world;

            _world_to_local = other._world_to_local;

            _dirty = other._dirty;

            if (other.parent)
            {
                other.parent->AddChild(this);
            }
            else
            {
                root = this;
                parent = nullptr;
            }

            _dirty = true;

            // we don't copy over children
        }

        void Transform::AddChild(Transform *child)
        {
            _children.push_back(child);
            child->parent = this;
            child->root = root;
            child->setAllChildrenDirty();

            // Set the child's new local positions
            child->SetPosition(ToLocal(child->position));
            child->SetRotation(child->rotation - rotation);
            SVec3 new_scale = child->scale / scale;
            child->SetScale(new_scale);
        }

        void Transform::AddChildAlreadyInLocal(Transform *child)
        {
            _children.push_back(child);
            child->parent = this;
            child->root = root;
            child->setAllChildrenDirty();

            child->SetPosition(child->position);
            child->SetRotation(child->rotation);
            child->SetScale(child->scale);
        }

        // helper for clearing child properties when it is removed from a hierarchy
        void Transform::clearRemovedChild(Transform *child)
        {
            child->parent = nullptr;
            child->root = child;
            child->_dirty = true;
        }

        // helper for clearing child properties when it is moved up the chain in a hierarchy
        void Transform::clearDetachedChild(Transform *child)
        {
            if (parent)
                parent->AddChild(child);
            else
                clearRemovedChild(child);
        }

        // Remove a child from the hierarchy
        void Transform::RemoveChild(Transform *child)
        {
            auto node = find(_children.begin(), _children.end(), child);

            if (node != _children.end())
            {
                _children.erase(node);
                clearRemovedChild(child);
            }
        }

        // Remove all children from the hierarchy
        void Transform::RemoveChildren(void)
        {
            for (Transform *child : _children)
                clearRemovedChild(child);

            _children.clear();
        }

        // Detach a child from this and attach to parent (move up hierarchy)
        void Transform::DetachChild(Transform *child)
        {
            auto node = find(_children.begin(), _children.end(), child);

            if (node != _children.end())
            {
                _children.erase(node);
                clearDetachedChild(child);
            }
        }

        // Detach all children from this and attach to parent (move up hierarchy)
        void Transform::DetachChildren(void)
        {
            for (Transform *child : _children)
                clearDetachedChild(child);

            _children.clear();
        }

        const SMat3 &Transform::Matrix(void)
        {
            if (_dirty)
            {
                if (parent)
                    _local_to_world = parent->Matrix() *
                        SMat3(Vec2(position), rotation.Z(), Vec2(scale));
                else
                    _local_to_world = SMat3(Vec2(position), rotation.Z(), Vec2(scale));

                _dirty = false;
            }

            return _local_to_world;
        }

        SMat3 Transform::MatrixNoScalar(void)
        {
            if (parent)
                return parent->MatrixNoScalar() * SMat3(Vec2(position), rotation.Z(), Vec2::One());
            else
                return SMat3(Vec2(position), rotation.Z(), Vec2::One());
        }

        const SMat3 &Transform::WorldToLocalMatrix(void)
        {
            if (_dirty)
            {
                _world_to_local = SMat3::Inverse(Matrix());

                _dirty = false;
            }

            return _world_to_local;
        }

        URSINE_TODO("Test this on children.");

        void Transform::LookAt2D(Vec2 point)
        {
            updateWorldPosition();

            Vec2 dir = point - (Vec2(_world_position));

            float angle = atan2f(dir.X(), dir.Y());

            SetRotation2D(angle);
        }

        bool Transform::IsChildOf(Transform *Parent)
        {
            return (parent == Parent);
        }

        Transform *Transform::GetChild(uint index)
        {
            uint count = 0;

            if (index < 0 || index >= _children.size())
                return nullptr;

            for (Transform *child : _children)
                if (index == count++)
                    return child;

            return nullptr;
        }

        const Transform *Transform::GetChild(uint index) const
        {
            uint count = 0;

            if (index < 0 || index >= _children.size())
                return nullptr;

            for (Transform *child : _children)
                if (index == count++)
                    return nullptr;

            return nullptr;
        }

        // return -1 if there isn't a parent
        int Transform::GetSiblingIndex(void) const
        {
            if (parent)
            {
                int count = 0;

                for (Transform *child : parent->_children)
                {
                    if (this == child)
                        return count;

                    ++count;
                }
            }

            return -1;
        }

        void Transform::updateWorldPosition(void)
        {
            _dirty = true;

            if (root != this)
                _world_position = root->ToWorld(SVec3(position.X(), position.Y(), 1));
            else
                _world_position = position;

            for (Transform *child : _children)
                child->updateWorldPosition();
        }

        void Transform::updateWorldRotation(void)
        {
            _dirty = true;

            // wrap rotations
            rotation.X() = Math::Wrap(rotation.X(), 0.0f, Math::PI_2);
            rotation.Y() = Math::Wrap(rotation.Y(), 0.0f, Math::PI_2);
            rotation.Z() = Math::Wrap(rotation.Z(), 0.0f, Math::PI_2);

            if (root != this)
                _world_rotation = root->_world_rotation * SMat3(rotation.Z());
            else
                _world_rotation = SMat3(rotation.Z());

            for (Transform *child : _children)
                child->updateWorldRotation();
        }

        void Transform::updateWorldScale(void)
        {
            _dirty = true;

            if (root != this)
                _world_scale = root->_world_scale * scale;
            else
                _world_scale = scale;

            for (Transform *child : _children)
                child->updateWorldScale();
        }

        void Transform::setAllChildrenDirty(void)
        {
            for (auto child : _children)
            {
                child->setAllChildrenDirty();
                child->_dirty = true;
            }
        }
    }
}
