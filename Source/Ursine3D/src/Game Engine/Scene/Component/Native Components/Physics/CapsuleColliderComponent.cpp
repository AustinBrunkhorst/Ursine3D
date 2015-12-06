/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CapsuleColliderComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "CapsuleColliderComponent.h"
#include "RigidbodyComponent.h"
#include "BodyComponent.h"
#include "EntityEvent.h"
#include "PhysicsSystem.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( CapsuleCollider );

        CapsuleCollider::CapsuleCollider(void)
            : BaseComponent( )
            , m_height( 1.0f )
            , m_radius( 0.5f )
        {

        }

        CapsuleCollider::~CapsuleCollider(void)
        {
            GetOwner( )->Listener( this )
                .Off( ENTITY_TRANSFORM_DIRTY, &CapsuleCollider::onTransformChange );
        }

        void CapsuleCollider::OnInitialize(void)
        {
            GetOwner( )->Listener( this )
                .On( ENTITY_TRANSFORM_DIRTY, &CapsuleCollider::onTransformChange );

            SetHeight( m_height );
            SetRadius( m_radius );
        }

        float CapsuleCollider::GetRadius(void) const
        {
            return m_radius;
        }

        void CapsuleCollider::SetRadius(float radius)
        {
            m_radius = radius;

            NOTIFY_COMPONENT_CHANGED( "radius", m_radius );

            updateHeightAndRadius( );
        }

        float CapsuleCollider::GetHeight(void) const
        {
            return m_height;
        }

        void CapsuleCollider::SetHeight(float height)
        {
            m_height = height;

            NOTIFY_COMPONENT_CHANGED( "height", m_height);

            updateHeightAndRadius( );
        }

        SVec3 CapsuleCollider::GetOffset(void) const
        {
            auto owner = GetOwner( );

            if (owner->HasComponent<Rigidbody>( ))
                return owner->GetComponent<Rigidbody>( )->GetOffset( );
            else
                return owner->GetComponent<Body>( )->GetOffset( );
        }

        void CapsuleCollider::SetOffset(const SVec3& offset)
        {
            auto owner = GetOwner( );

            if (owner->HasComponent<Rigidbody>( ))
                return owner->GetComponent<Rigidbody>( )->SetOffset( offset );
            else
                return owner->GetComponent<Body>( )->SetOffset( offset );
        }

        void CapsuleCollider::onTransformChange(EVENT_HANDLER(Entity))
        {
            EVENT_ATTRS(Entity, TransformChangedArgs);

            if (args->scaleChanged)
                updateHeightAndRadius( );
        }

        void CapsuleCollider::updateHeightAndRadius(void)
        {
            auto owner = GetOwner( );

            auto scale = owner->GetTransform( )->GetWorldScale( );

            auto radius = std::max( scale.X( ), scale.Z( ) ) * m_radius;
            auto height = scale.Y( ) * m_height;

            m_capsuleCollider.SetRadius( radius );
            m_capsuleCollider.SetHeight( height );

            auto rigidbody = owner->GetComponent<Rigidbody>( );

            if (rigidbody)
            {
                GetOwner( )->GetWorld( )->GetEntitySystem( PhysicsSystem )
                    ->ClearContacts( rigidbody );

                rigidbody->SetAwake( );

                rigidbody->UpdateInertiaTensor();
            }
        }
    }
}
