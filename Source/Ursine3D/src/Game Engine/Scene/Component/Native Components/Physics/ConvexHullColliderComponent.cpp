/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ConvexHullColliderComponent.cpp
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "ConvexHullColliderComponent.h"
#include "RigidbodyComponent.h"
#include "EntityEvent.h"
#include "PhysicsSystem.h"

namespace ursine
{
	namespace ecs
	{
		NATIVE_COMPONENT_DEFINITION(ConvexHullCollider);


		ConvexHullCollider::ConvexHullCollider(void)
			: BaseComponent( )
		{
		}

		ConvexHullCollider::~ConvexHullCollider(void)
		{
			GetOwner( )->Listener( this )
				.Off( ENTITY_TRANSFORM_DIRTY, &ConvexHullCollider::onTransformChange );
		}

		void ConvexHullCollider::OnInitialize(void)
		{
			GetOwner( )->Listener( this )
				.On( ENTITY_TRANSFORM_DIRTY, &ConvexHullCollider::onTransformChange );
		}

		void ConvexHullCollider::GenerateConvexHull(Model3D* model)
		{
			m_convexHullCollider.GenerateConvexHull( model );

			m_convexHullCollider.SetScale( GetOwner( )->GetTransform( )->GetWorldScale( ) );
		}

		void ConvexHullCollider::ReduceVertices(void)
		{
			m_convexHullCollider.ReduceVertices( );
		}

		float ConvexHullCollider::GetMargin(void) const
		{
			return m_convexHullCollider.GetMargin( );
		}

		void ConvexHullCollider::SetMargin(float margin)
		{
			if (margin < 0.0f)
				return;

			m_convexHullCollider.SetMargin( margin );
		}

		void ConvexHullCollider::onTransformChange(EVENT_HANDLER(Entity))
		{
			EVENT_ATTRS(Entity, TransformChangedArgs);

			if (!args->scaleChanged)
				return;

			m_convexHullCollider.SetScale( GetOwner( )->GetTransform( )->GetWorldScale( ) );

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
