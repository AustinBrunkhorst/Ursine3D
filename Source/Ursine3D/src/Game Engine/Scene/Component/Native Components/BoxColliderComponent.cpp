#include "UrsinePrecompiled.h"

#include "BoxColliderComponent.h"
#include "RigidbodyComponent.h"
#include "EntityEvent.h"
#include "PhysicsSystem.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( BoxCollider );

        BoxCollider::BoxCollider(void)
            : BaseComponent( )
            , m_dimensions(1, 1, 1) { }

        BoxCollider::~BoxCollider(void)
        {
            GetOwner( )->Listener( this )
                .Off( ENTITY_TRANSFORM_DIRTY, &BoxCollider::onTransformChange );
        }

        void BoxCollider::OnInitialize(void)
        {
            GetOwner( )->Listener( this )
                .On( ENTITY_TRANSFORM_DIRTY, &BoxCollider::onTransformChange );

            SetDimensions( m_dimensions );
        }

        SVec3 BoxCollider::GetDimensions(void) const
        {
            return m_dimensions;
        }

        void BoxCollider::SetDimensions(const SVec3 &dimensions)
        {
            if (dimensions.X( ) > 0.01f &&
                dimensions.Y( ) > 0.01f &&
                dimensions.Z( ) > 0.01f)
            {
                m_dimensions = dimensions;
            }

            NOTIFY_COMPONENT_CHANGED( "dimensions", m_dimensions );

            updateDimensions( );
        }

        void BoxCollider::onTransformChange(EVENT_HANDLER(Entity))
        {
            EVENT_ATTRS(Entity, TransformChangedArgs);

            if (args->scaleChanged)
                updateDimensions( );
        }

        void BoxCollider::updateDimensions(void)
        {
            m_boxCollider.SetDimensions(
                m_dimensions * GetOwner( )->GetTransform( )->GetWorldScale( ) 
            );

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
