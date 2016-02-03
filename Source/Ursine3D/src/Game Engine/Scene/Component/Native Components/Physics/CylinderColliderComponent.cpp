/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CylinderColliderComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "CylinderColliderComponent.h"
#include "RigidbodyComponent.h"
#include "BodyComponent.h"
#include "EntityEvent.h"
#include "PhysicsSystem.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( CylinderCollider );

        CylinderCollider::CylinderCollider(void)
            : BaseComponent( )
            , m_height( 1.0f )
            , m_radius( 0.5f )
        {
            
        }

        CylinderCollider::~CylinderCollider(void)
        {
            GetOwner( )->Listener( this )
                .Off( ENTITY_TRANSFORM_DIRTY, &CylinderCollider::onTransformChange );
        }

        void CylinderCollider::OnInitialize(void)
        {
            GetOwner( )->Listener( this )
                .On( ENTITY_TRANSFORM_DIRTY, &CylinderCollider::onTransformChange );

            SetHeight( m_height );
            SetRadius( m_radius );
        }

        float CylinderCollider::GetRadius(void) const
        {
            return m_radius;
        }

        void CylinderCollider::SetRadius(float radius)
        {
            m_radius = radius;

            NOTIFY_COMPONENT_CHANGED( "radius", m_radius );

            updateHeightAndRadius( );
        }

        float CylinderCollider::GetHeight(void) const
        {
            return m_height;
        }

        void CylinderCollider::SetHeight(float height)
        {
            m_height = height;

            NOTIFY_COMPONENT_CHANGED( "height", m_height );

            updateHeightAndRadius( );
        }

        void CylinderCollider::onTransformChange(EVENT_HANDLER(Entity))
        {
            EVENT_ATTRS(Entity, TransformChangedArgs);

            if (args->scaleChanged)
                updateHeightAndRadius( );
        }

        void CylinderCollider::updateHeightAndRadius(void)
        {
            auto scale = GetOwner( )->GetTransform( )->GetWorldScale( );

            auto radius = std::max( scale.X( ), scale.Z( ) ) * m_radius;
            auto height = scale.Y( ) * m_height;

            m_cylinderCollider.SetRadius( radius );
            m_cylinderCollider.SetHeight( height );

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
