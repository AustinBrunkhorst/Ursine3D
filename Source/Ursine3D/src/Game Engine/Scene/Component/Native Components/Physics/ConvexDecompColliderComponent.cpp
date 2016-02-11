/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ConvexDecompColliderComponent.cpp
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "ConvexDecompColliderComponent.h"
#include "RigidbodyComponent.h"

#include "PhysicsSystem.h"
#include "EntityEvent.h"

namespace ursine
{
	namespace ecs
	{
		NATIVE_COMPONENT_DEFINITION(ConvexDecompCollider);

		ConvexDecompCollider::ConvexDecompCollider(void)
			: BaseComponent( )
		{
			SetMargin( 0.04f );
		}

		ConvexDecompCollider::~ConvexDecompCollider(void)
		{
			GetOwner( )->Listener( this )
				.Off( ENTITY_TRANSFORM_DIRTY, &ConvexDecompCollider::onTransformChange );
		}

		void ConvexDecompCollider::OnInitialize(void)
		{
			GetOwner( )->Listener( this )
				.On( ENTITY_TRANSFORM_DIRTY, &ConvexDecompCollider::onTransformChange );
		}

		void ConvexDecompCollider::OnSerialize(Json::object &output) const
		{
			m_convexDecompCollider.SetScale( SVec3( 1.0f, 1.0f, 1.0f ) );

			m_convexDecompCollider.Serialize( output );

			m_convexDecompCollider.SetScale( GetOwner( )->GetTransform( )->GetWorldScale( ) );
		}

		void ConvexDecompCollider::OnDeserialize(const Json &input)
		{
			m_convexDecompCollider.Deserialize( input );

			m_convexDecompCollider.SetScale( GetOwner( )->GetTransform( )->GetWorldScale( ) );
		}

		void ConvexDecompCollider::GenerateConvexHulls(Model3D *model)
		{
			if (!model)
				return;

			m_convexDecompCollider.SetScale( SVec3( 1.0f, 1.0f, 1.0f ) );

			m_convexDecompCollider.GenerateConvexHulls( model );

			m_convexDecompCollider.SetScale( GetOwner( )->GetTransform( )->GetWorldScale( ) );
		}

		float ConvexDecompCollider::GetMargin(void) const
		{
			return m_convexDecompCollider.GetMargin( );
		}

		void ConvexDecompCollider::SetMargin(float margin)
		{
			m_convexDecompCollider.SetMargin( margin );
		}

		void ConvexDecompCollider::onTransformChange(EVENT_HANDLER(Entity))
		{
			EVENT_ATTRS(Entity, TransformChangedArgs);

			if (!args->scaleChanged)
				return;

			auto owner = GetOwner( );

			m_convexDecompCollider.SetScale( owner->GetTransform( )->GetWorldScale( ) );

			auto rigidbody = owner->GetComponent<Rigidbody>( );

			if (rigidbody)
			{
				owner->GetWorld( )->GetEntitySystem( PhysicsSystem )
					->ClearContacts( rigidbody );

				rigidbody->SetAwake( );
				rigidbody->UpdateInertiaTensor( );
			}
		}
	}
}