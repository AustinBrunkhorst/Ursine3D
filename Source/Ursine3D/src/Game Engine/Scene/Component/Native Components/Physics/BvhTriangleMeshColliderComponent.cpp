/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BvhTriangleMeshColliderComponent.cpp
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "BvhTriangleMeshColliderComponent.h"
#include "RigidbodyComponent.h"

#include "PhysicsSystem.h"
#include "EntityEvent.h"


namespace ursine
{
	namespace ecs
	{
		NATIVE_COMPONENT_DEFINITION(BvhTriangleMeshCollider);

		BvhTriangleMeshCollider::BvhTriangleMeshCollider(void)
			: BaseComponent( )
		{
		}

		BvhTriangleMeshCollider::~BvhTriangleMeshCollider(void)
		{
			GetOwner( )->Listener( this )
				.Off( ENTITY_TRANSFORM_DIRTY, &BvhTriangleMeshCollider::onTransformChange );
		}

		void BvhTriangleMeshCollider::OnInitialize()
		{
			GetOwner( )->Listener( this )
				.On( ENTITY_TRANSFORM_DIRTY, &BvhTriangleMeshCollider::onTransformChange );
		}

		void BvhTriangleMeshCollider::OnSerialize(Json::object& output) const
		{
			m_bvhTriangleMeshCollider.Serialize( output );
		}

		void BvhTriangleMeshCollider::OnDeserialize(const Json& input)
		{
			m_bvhTriangleMeshCollider.Deserialize( input );

			m_bvhTriangleMeshCollider.SetScale( GetOwner( )->GetTransform( )->GetWorldScale( ) );
		}

		void BvhTriangleMeshCollider::GenerateBvhTriangleMesh(Model3D* model)
		{
			if (!model)
				return;

			m_bvhTriangleMeshCollider.GenerateBvhTriangleMesh( model );

			m_bvhTriangleMeshCollider.SetScale( GetOwner( )->GetTransform( )->GetWorldScale( ) );
		}

		void BvhTriangleMeshCollider::onTransformChange(EVENT_HANDLER(Entity))
		{
			EVENT_ATTRS(Entity, TransformChangedArgs);

			if (!args->scaleChanged)
				return;

			m_bvhTriangleMeshCollider.SetScale( GetOwner( )->GetTransform( )->GetWorldScale( ) );

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
