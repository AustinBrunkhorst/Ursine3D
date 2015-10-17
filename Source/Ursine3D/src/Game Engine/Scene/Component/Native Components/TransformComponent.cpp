#include "UrsinePrecompiled.h"

#include "TransformComponent.h"
#include "Entity.h"
#include "EntityEvent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( Transform );

        Transform::Transform(void)
            : BaseComponent( )
            , m_dirty( true )
        {
        }

        Transform::Transform(const Transform &transform)
            : Component( transform )
        {
            copy( transform );
        }

        Transform::~Transform(void)
        {

        }

        Transform &Transform::operator=(const Transform &transform)
        {
            copy( transform );

            return *this;
        }

        void Transform::OnInitialize(void)
        {

        }

        const SVec3 &Transform::GetWorldPosition(void) const
        {
            return m_worldPosition;
        }

        void Transform::SetWorldPosition(const SVec3 &position)
        {
            m_worldPosition = position;

            dispatch( );

            NOTIFY_COMPONENT_CHANGED( "position", m_worldPosition );
        }

        const SQuat &Transform::GetWorldRotation(void) const
        {
            return m_worldRotation;
        }

        void Transform::SetWorldRotation(const SQuat &rotation)
        {
            m_worldRotation = rotation;

            dispatch( );

            NOTIFY_COMPONENT_CHANGED( "rotation", m_worldRotation.GetEulerAngles( ) );
        }

        SVec3 Transform::GetWorldEuler(void) const
        {
            return m_worldRotation.GetEulerAngles( );
        }

        void Transform::SetWorldEuler(const SVec3 &euler)
        {
            m_worldRotation.SetEulerAngles( euler );

            dispatch( );

            NOTIFY_COMPONENT_CHANGED( "rotation", euler );
        }

        void Transform::editorSetRotation(const SVec3 &euler)
        {
            m_worldRotation.SetEulerAngles( euler );

            dispatch( );

            NOTIFY_COMPONENT_CHANGED( "rotation", euler );
        }

        const SVec3 &Transform::GetWorldScale(void) const
        {
            return m_worldScale;
        }

        void Transform::SetWorldScale(const SVec3 &scale)
        {
            m_worldScale = scale;

            dispatch( );

            NOTIFY_COMPONENT_CHANGED( "scale", scale );
        }

        void Transform::copy(const Transform &transform)
        {
            m_dirty = transform.m_dirty;
            m_worldPosition = transform.m_worldPosition;
            m_worldRotation = transform.m_worldRotation;
            m_worldScale = transform.m_worldScale;

            dispatch( );
        }

        void Transform::dispatch(void) const
        {
            GetOwner( )->Dispatch( ENTITY_TRANSFORM_CHANGED, EventArgs::Empty );
        }
    }
}
