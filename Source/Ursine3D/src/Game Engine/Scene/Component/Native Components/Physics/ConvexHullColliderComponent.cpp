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

		void ConvexHullCollider::OnSerialize(Json::object& output) const
		{
			m_convexHullCollider.Serialize( output );
		}

		void ConvexHullCollider::OnDeserialize(const Json& input)
		{
			m_convexHullCollider.Deserialize( input );

			m_convexHullCollider.SetScale( GetOwner( )->GetTransform( )->GetWorldScale( ) );
		}

		void ConvexHullCollider::GenerateConvexHull(Model3D* model)
		{
			if (!model)
				return;

			m_convexHullCollider.GenerateConvexHull( model );

			m_convexHullCollider.SetScale( GetOwner( )->GetTransform( )->GetWorldScale( ) );
		}

		void ConvexHullCollider::ReduceVertices(void)
		{
			m_convexHullCollider.SetScale( SVec3( 1.0f, 1.0f, 1.0f ) );
			m_convexHullCollider.ReduceVertices( );
			m_convexHullCollider.SetScale( GetOwner( )->GetTransform( )->GetWorldScale( ) );
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

			auto owner = GetOwner( );

			m_convexHullCollider.SetScale( owner->GetTransform( )->GetWorldScale( ) );

			auto rigidbody = owner->GetComponent<Rigidbody>( );

            if (rigidbody)
            {
				owner->GetWorld( )->GetEntitySystem( PhysicsSystem )
                    ->ClearContacts( rigidbody );

                rigidbody->SetAwake( );
                rigidbody->UpdateInertiaTensor( );
            }
		}

	#if defined(URSINE_WITH_EDITOR)

		void ConvexHullCollider::ReduceConvexHull(void)
        {
			ReduceVertices( );
        }

	#endif
	}
}
