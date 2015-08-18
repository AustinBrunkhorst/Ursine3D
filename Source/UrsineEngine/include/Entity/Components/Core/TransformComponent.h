/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TransformComponent.h
** - Transform component
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

////////////////////////////////////////////////////////////////////////////////
#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H
////////////////////////////////////////////////////////////////////////////////

#include "Vec3.h"
#include "Vec2.h"
#include "Mat3.h"
#include "Component.h"

namespace Ursine
{
    namespace ECS
    {
        class Transform : public Component
        {
            // World coordinates.
            Vec3 _world_position,
                    _world_scale;
            Mat3 _world_rotation;

            SerializerAllow(Transform, position);
            SerializerAllow(Transform, rotation);
            SerializerAllow(Transform, scale);

            // Local coordinates (coordinates in relation to the parent).
            Vec3 position,
                    // Euler angles will be used (in radians).
                    rotation,
                    scale;

            // Transformation from local space into world space.
            Mat3 _local_to_world;

            // Transformation from world space into local space.
            Mat3 _world_to_local;

            // The top most transform in the hierarchy.
            // If this is the top most, root == this.
            Transform *root;

            // The parent of this transform.
            // If there is no parent, parent == nullptr.
            Transform *parent;

            // Child pointers.
            std::vector<Transform*> _children;

            // Flag telling us if this object's data has been changed this frame.
            bool _dirty;

            // Helpers.
            void copy(const Transform &other);
            
            void clearRemovedChild(Transform *child);
            void clearDetachedChild(Transform *child);

            void updateWorldPosition(void);
            void updateWorldRotation(void);
            void updateWorldScale(void);

            void setAllChildrenDirty(void);

        public:
            Transform(void);
            Transform(const Transform &transform);

            ~Transform(void);

            Transform &operator=(const Transform &transform);

            // Initializer after serialization phase
            void OnInitialize(void) override;

            ////////////////////////////////////////////////////////////////////
            // Position
            ////////////////////////////////////////////////////////////////////

            // Set the position of the Root
            inline void SetRootPosition(const Vec3 &position);
            inline void AddRootPosition(const Vec3 &position);

            // Set the local position
            inline void SetPosition(const Vec3 &position);
            inline void AddPosition(const Vec3 &position);

            // Set the position in world coordinates, neglecting the parent's scale
            inline void SetPositionWC(const Vec3 &position);
            inline void AddPositionWC(const Vec3 &position);

            // Get the position in world coordinates
            inline const Vec3 &PositionWC(void) const;

            // Get the local position
            inline const Vec3 &LocalPosition(void) const;

            ////////////////////////////////////////////////////////////////////
            // Rotation
            ////////////////////////////////////////////////////////////////////

            // Set the rotation of the Root
            inline void SetRootRotation(const Vec3 &rotation);
            inline void AddRootRotation(const Vec3 &rotation);

            // Set the local rotation
            inline void SetRotation(const Vec3 &rotation);
            inline void AddRotation(const Vec3 &rotation);

            // Get the rotation matrix in world coordinates
            inline const Mat3 &RotationWC(void) const;

            // Get the local rotation
            inline const Vec3 &LocalRotation(void) const;

            // Set the root's 2D rotation
            inline void SetRootRotation2D(float radians);
            inline void AddRootRotation2D(float radians);

            // Set the local 2D rotation
            inline void SetRotation2D(float radians);
            inline void AddRotation2D(float radians);

            // Get the rotation in radians of the root
            inline float RootRotation2D(void) const;
            inline Vec2 RootRotation2DVec(void) const;

            // Get the local rotation in radians
            inline float LocalRotation2D(void) const;
            inline Vec2 LocalRotation2DVec(void) const;

            ////////////////////////////////////////////////////////////////////
            // Scale
            ////////////////////////////////////////////////////////////////////

            // Set the scale of the Root
            inline void SetRootScale(const Vec3 &scale);
            inline void AddRootScale(const Vec3 &scale);

            // Set the local scale
            inline void SetScale(const Vec3 &scale);
            inline void AddScale(const Vec3 &scale);

            // Set the scale in world coordinates, neglecting the parent's scale
            inline void SetScaleWC(const Vec3 &scale);
            inline void AddScaleWC(const Vec3 &scale);

            // Get the scale of the object in world coordinates
            inline const Vec3 &ScaleWC(void) const;

            // Get the local scale of the object
            inline const Vec3 &LocalScale(void) const;

            ////////////////////////////////////////////////////////////////////
            // Matrix
            ////////////////////////////////////////////////////////////////////

            inline bool Dirty(void) const;

            const Mat3 &Matrix(void);
            Mat3 MatrixNoScalar(void);
            const Mat3 &WorldToLocalMatrix(void);

            inline Vec3 ToLocal(const Vec3 &point);
            inline Vec2 ToLocal(const Vec2 &point);

            inline Vec3 ToWorld(const Vec3 &point);
            inline Vec2 ToWorld(const Vec2 &point);

            void LookAt2D(Vec2 point);

            ////////////////////////////////////////////////////////////////////
            // Hierarchy Related
            ////////////////////////////////////////////////////////////////////

            inline Transform *Root(void);
            inline Transform *Parent(void);
            
            bool IsChildOf(Transform *parent);

            // Add a child to the hierarchy, assuming its coordinates are in world space
            void AddChild(Transform *child);

            // Add a child to the hierarchy, assuming its coordinates are in local space already
            void AddChildAlreadyInLocal(Transform *child);

            // Remove a child from the hierarchy
            void RemoveChild(Transform *child);
            // Remove all children from the hierarchy
            void RemoveChildren(void);

            // Detach a child from this and attach to parent (move up hierarchy)
            void DetachChild(Transform *child);
            // Detach all children from this and attach to parent (move up hierarchy)
            void DetachChildren(void);

            // Find child by entity tag name
            //int FindChild();

            // Find child by their index in the list
            // If the index is too large, return this
            Transform *GetChild(uint index);
            const Transform *GetChild(uint index) const;

            // Find this transform's index in the parent's children list
            int GetSiblingIndex(void) const;

            // moves the transform to the start of the local transform list
            //void SetAsFirstSibling();

            // sets the transform to the index of the local transform list
            //void SetSiblingIndex();

            // Call a certain member function on all children in the transform
            // hierarchy, and forwards the specified args.
            template<class ComponentType, typename... Args>
            void ApplyChildren(void (ComponentType::*)(Args...), Args&&...);
        };
    }
}

#include "TransformComponent.hpp"

////////////////////////////////////////////////////////////////////////////////
#endif // !TRANSFORM_COMPONENT_H
////////////////////////////////////////////////////////////////////////////////

#ifdef REGISTER_COMPONENTS
#ifndef TRANSFORM_REGISTERED
#define TRANSFORM_REGISTERED
namespace Ursine
{
    namespace ECS
    {
        RegisterComponent(Transform);
        RegisterComponentData(Transform, position);
        RegisterComponentData(Transform, rotation);
        RegisterComponentData(Transform, scale);
    }
}
#endif
#endif
