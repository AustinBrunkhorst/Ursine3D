#include "UrsinePrecompiled.h"

#include "ConeColliderComponent.h"
#include "RigidbodyComponent.h"
#include "EntityEvent.h"
#include "PhysicsSystem.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( ConeCollider );

        ConeCollider::ConeCollider(void)
            : BaseComponent( )
            , m_height( 1.0f )
            , m_radius( 0.5f )
        {
            
        }

        ConeCollider::~ConeCollider(void)
        {
            GetOwner( )->Listener( this )
                .Off( ENTITY_TRANSFORM_DIRTY, &ConeCollider::onTransformChange );
        }

        void ConeCollider::OnInitialize(void)
        {
            GetOwner( )->Listener( this )
                .On( ENTITY_TRANSFORM_DIRTY, &ConeCollider::onTransformChange );

            SetHeight( m_height );
            SetRadius( m_radius );
        }

        float ConeCollider::GetRadius(void) const
        {
            return m_radius;
        }

        void ConeCollider::SetRadius(float radius)
        {
            m_radius = radius;

            NOTIFY_COMPONENT_CHANGED( "radius", m_radius );

            updateHeightAndRadius( );
        }

        float ConeCollider::GetHeight(void) const
        {
            return m_height;
        }

        void ConeCollider::SetHeight(float height)
        {
            m_height = height;

            NOTIFY_COMPONENT_CHANGED( "height", m_height );

            updateHeightAndRadius( );
        }

        void ConeCollider::onTransformChange(EVENT_HANDLER(Entity))
        {
            EVENT_ATTRS(Entity, TransformChangedArgs);

            if (args->scaleChanged)
                updateHeightAndRadius( );
        }

        void ConeCollider::updateHeightAndRadius(void)
        {
            auto scale = GetOwner( )->GetTransform( )->GetWorldScale( );

            auto radius = std::max( scale.X( ), scale.Z( ) ) * m_radius;
            auto height = scale.Y( ) * m_height;

            m_coneCollider.SetRadius( radius );
            m_coneCollider.SetHeight( height );

            auto rigidbody = GetOwner( )->GetComponent<Rigidbody>( );

            if (rigidbody)
            {
                GetOwner( )->GetWorld( )->GetEntitySystem( PhysicsSystem )
                    ->ClearContacts( rigidbody );

                rigidbody->SetAwake( );
            }
        }
    }
}
