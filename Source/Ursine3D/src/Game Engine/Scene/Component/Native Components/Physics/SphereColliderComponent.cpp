/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SphereColliderComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "SphereColliderComponent.h"
#include "RigidbodyComponent.h"
#include "EntityEvent.h"
#include "PhysicsSystem.h"

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
            Component::OnInitialize( );

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
            EVENT_ATTRS(Entity, TransformChangedArgs);

            if (args->scaleChanged)
                updateRadius( );
        }

        void SphereCollider::updateRadius(void)
        {
            auto scale = GetOwner( )->GetTransform( )->GetWorldScale( );
            auto max = std::max( std::max( scale.X( ), scale.Y( ) ), scale.Z( ) );

            m_sphereCollider.SetRadius( m_radius * max );

            auto rigidbody = GetOwner( )->GetComponent<Rigidbody>( );

            if (rigidbody)
            {
                GetOwner( )->GetWorld( )->GetEntitySystem( PhysicsSystem )
                    ->ClearContacts( rigidbody );

                rigidbody->SetAwake( );

                rigidbody->UpdateInertiaTensor( );
            }
        }
    }
}
