#pragma once

#include "Component.h"
#include "SVec3.h"
#include "SQuat.h"
#include "SMat4.h"

namespace ursine
{
    namespace ecs
    {
        ALIGNED16(class) Transform : public Component
        {
            NATIVE_COMPONENT;

        public:
        #if defined(__REFLECTION_PARSER__)

            Meta(Getter( "GetterForfieldName" ), Setter( "SetterForfieldName" ))
            SVec3 fieldName;

        #endif

            Transform(void);
            Transform(const Transform &transform);

            ~Transform(void);

            Transform &operator=(const Transform &transform);

            void OnInitialize(void) override;

            ////////////////////////////////////////////////////////////////////
            // Position
            ////////////////////////////////////////////////////////////////////

            // Set the local position
            void SetLocalPosition(const SVec3 &position);

            const SVec3 &LocalPosition(void) const;

            // Set the position in world coordinates, neglecting the parent's scale
            void SetWorldPosition(const SVec3 &position);

            const SVec3 &WorldPosition(void) const;

            ////////////////////////////////////////////////////////////////////
            // Rotation
            ////////////////////////////////////////////////////////////////////

            // Set the local rotation
            void SetLocalRotation(const SQuat &rotation);
            void SetLocalEuler(const SVec3 &euler);

            const SQuat &LocalRotation(void) const;
            SVec3 LocalEuler(void) const;

            // Set the world rotation
            void SetWorldRotation(const SQuat &rotation);
            void SetWorldEuler(const SVec3 &euler);

            const SQuat &WorldRotation(void) const;
            SVec3 WorldEuler(void) const;

            ////////////////////////////////////////////////////////////////////
            // Scale
            ////////////////////////////////////////////////////////////////////

            // Set the local scale
            void SetLocalScale(const SVec3 &scale);
            const SVec3 &LocalScale(void) const;

            ////////////////////////////////////////////////////////////////////
            // Axis
            ////////////////////////////////////////////////////////////////////

            SVec3 Forward(void) const;
            SVec3 Right(void) const;
            SVec3 Up(void) const;

        protected:

            // The top most transform in the hierarchy
            // If this is the top most, root == this
            // Transform *m_root;

            // The parent of this transform.
            // If there is no parent, parent == nullptr
            // Transform *m_parent;

            // Child pointers.
            // std::vector<Transform*> _children;

            // Flag telling us if this object's data has been changed this frame
            bool m_dirty;

            // World coordinates
            SVec3 m_world_position,
                m_world_scale;

            SQuat m_world_rotation;

            // Local coordinates (coordinates in relation to the parent)
            SVec3 m_local_position,
                  m_local_scale;

            SQuat m_local_rotation;

        } Meta(Enable, DisplayName( "Transform" ));
    }
}