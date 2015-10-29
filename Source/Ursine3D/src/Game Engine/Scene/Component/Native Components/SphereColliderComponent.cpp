#include "UrsinePrecompiled.h"

#include "SphereColliderComponent.h"
#include "RigidbodyComponent.h"
#include "EntityEvent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( SphereCollider );

        SphereCollider::SphereCollider(void)
            : BaseComponent( ) 
            , m_radius( 0.5f ) { }

        SphereCollider::~SphereCollider(void)
        {
            GetOwner( )->Listener( this )
                .Off( ENTITY_TRANSFORM_DIRTY, &SphereCollider::onTransformChange );
        }

        void SphereCollider::OnInitialize(void)
        {
            GetOwner( )->Listener( this )
                .On( ENTITY_TRANSFORM_DIRTY, &SphereCollider::onTransformChange );

            SetRadius( m_radius );
        }
        
        float SphereCollider::GetRadius(void) const
        {
            return m_radius;
        }

        void SphereCollider::SetRadius(float radius)
        {
            if (radius > 0.01f)
                m_radius = radius;

            NOTIFY_COMPONENT_CHANGED( "radius", m_radius );

            updateRadius( );
        }

        void SphereCollider::onTransformChange(EVENT_HANDLER(Entity))
        {
            updateRadius( );
        }

        void SphereCollider::updateRadius(void)
        {
            auto scale = GetOwner( )->GetTransform( )->GetWorldScale( );
            auto max = std::max( std::max( scale.X( ), scale.Y( ) ), scale.Z( ) );

            m_sphereCollider.SetRadius( m_radius * max );

            auto rigidbody = GetOwner( )->GetComponent<Rigidbody>( );

            if (rigidbody)
                rigidbody->SetAwake( );
        }
    }
}
