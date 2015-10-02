#include "UrsinePrecompiled.h"

#include "TransformComponent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( Transform );

        Transform::Transform(void)
            : BaseComponent()
        {
            
        }

        Transform::Transform(const Transform& transform)
            : Component( transform )
        {
            copy(transform);
        }

        Transform::~Transform(void)
        {
            
        }

        Transform &Transform::operator=(const Transform &transform)
        {
            copy(transform);

            return *this;
        }

        void Transform::OnInitialize(void)
        {
            
        }

        void Transform::SetWorldPosition(const SVec3 &position)
        {
            m_worldPosition = position;
        }

        const SVec3 &Transform::GetWorldPosition(void) const
        {
            return m_worldPosition;
        }

        const SVec3& Transform::editorGetTranslation(void) const
        {
            return m_worldPosition;
        }

        void Transform::editorSetTranslation(const SVec3& position)
        {
            m_worldPosition = position;
        }

        void Transform::SetWorldRotation(const SQuat &rotation)
        {
            m_worldRotation = rotation;
        }

        void Transform::SetWorldEuler(const SVec3 &euler)
        {
            m_worldRotation.SetEulerAngles( euler );
        }

        const SQuat &Transform::GetWorldRotation(void) const
        {
            return m_worldRotation;
        }

        SVec3 Transform::GetWorldEuler(void) const
        {
            return m_worldRotation.GetEulerAngles( );
        }

        SVec3 Transform::editorGetRotation(void) const
        {
            return m_worldRotation.GetEulerAngles();
        }

        void Transform::editorSetRotation(const SVec3& euler)
        {
            m_worldRotation.SetEulerAngles(euler);
        }

        void Transform::SetWorldScale(const SVec3 &scale)
        {
            m_worldScale = scale;
        }

        const SVec3 &Transform::GetWorldScale(void) const
        {
            return m_worldScale;
        }

        const SVec3& Transform::editorGetScale(void) const
        {
            return m_worldScale;
        }

        void Transform::editorSetScale(const SVec3& scale)
        {
            m_worldScale = scale;
        }

        void Transform::copy(const Transform &transform)
        {
            m_dirty = transform.m_dirty;
            m_worldPosition = transform.m_worldPosition;
            m_worldRotation = transform.m_worldRotation;
            m_worldScale = transform.m_worldScale;
        }
    }
}