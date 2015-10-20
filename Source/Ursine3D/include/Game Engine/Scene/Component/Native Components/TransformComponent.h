#pragma once

#include "Component.h"
#include "SVec3.h"
#include "SQuat.h"
#include "SMat4.h"

namespace ursine
{
    namespace ecs
    {
        class Transform : public Component
        {
            NATIVE_COMPONENT;

        public:
            EditorField( 
                SVec3 Translation, 
                GetWorldPosition, 
                SetWorldPosition 
            );

            EditorField( 
                SVec3 Rotation, 
                GetWorldEuler,
                editorSetRotation
            );

            EditorField( 
                SVec3 Scale, 
                GetWorldScale, 
                SetWorldScale 
            );

            Transform(void);

            Transform(const Transform &transform);

            ~Transform(void);

            Transform &operator=(const Transform &transform);

            void OnInitialize(void) override;

            ////////////////////////////////////////////////////////////////////
            // Position
            ////////////////////////////////////////////////////////////////////

            /*void SetLocalPosition(const SVec3 &position);

            const SVec3 &LocalPosition(void) const;*/

            void SetWorldPosition(const SVec3 &position);

            const SVec3 &GetWorldPosition(void) const;

            ////////////////////////////////////////////////////////////////////
            // Rotation
            ////////////////////////////////////////////////////////////////////

            /*void SetLocalRotation(const SQuat &rotation);
            void SetLocalEuler(const SVec3 &euler);

            const SQuat &LocalRotation(void) const;
            SVec3 LocalEuler(void) const;*/

            void SetWorldRotation(const SQuat &rotation);
            void SetWorldEuler(const SVec3 &euler);

            const SQuat &GetWorldRotation(void) const;
            SVec3 GetWorldEuler(void) const;

            void editorSetRotation(const SVec3 &euler);

            ////////////////////////////////////////////////////////////////////
            // Scale
            ////////////////////////////////////////////////////////////////////

            /*void SetLocalScale(const SVec3 &scale);
            
            const SVec3 &LocalScale(void) const;*/

            void SetWorldScale(const SVec3 &scale);

            const SVec3 &GetWorldScale(void) const;

            ////////////////////////////////////////////////////////////////////
            // Axis
            ////////////////////////////////////////////////////////////////////

            // Do this in the quaternion class
            /*SVec3 Forward(void) const;
            SVec3 Right(void) const;
            SVec3 Up(void) const;*/

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
            SVec3 m_worldPosition,
                  m_worldScale;

            SQuat m_worldRotation;

            // Local coordinates (coordinates in relation to the parent)
            /*SVec3 m_local_position,
                  m_local_scale;

            SQuat m_local_rotation;*/

        private:
            void copy(const Transform &transform);

            void dispatch(void) const;

        } Meta(Enable, WhiteListMethods, DisplayName( "Transform" ));
    }
}