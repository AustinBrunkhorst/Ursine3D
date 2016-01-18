/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BvhTriangleMeshCollider.cpp
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "BvhTriangleMeshCollider.h"

#include "Model3DComponent.h"

namespace ursine
{
	namespace physics
	{
	#ifdef BULLET_PHYSICS

		BvhTriangleMeshCollider::BvhTriangleMeshCollider(void)
			: BvhTriangleMeshColliderBase( new btTriangleIndexVertexArray( ), true, false )
		{
			m_triangleIndexVertexArray = reinterpret_cast<btTriangleIndexVertexArray*>( m_meshInterface );
		}

	#endif

		BvhTriangleMeshCollider::~BvhTriangleMeshCollider(void)
		{
		}

		void BvhTriangleMeshCollider::GenerateBvhTriangleMesh(ecs::Model3D* model)
		{
		#ifdef BULLET_PHYSICS

			m_triangleIndexVertexArray->getIndexedMeshArray( ).clear( );

			// Add all meshes of the model to the bvhTriangleMesh
			auto modelResource = model->GetModelResource( );

			if (!modelResource)
			{
				UWarning("This model doesn't have a ModelResource. Talk to Matt?");
				return;
			}

			auto meshArray = modelResource->GetMeshArray( );
			auto meshIndex = model->GetMeshIndex( );
			std::vector<graphics::Mesh*> meshesToAdd;

			if (meshIndex == -1)
				for (auto &mesh : meshArray)
					meshesToAdd.push_back( mesh );
			else
				meshesToAdd.push_back( meshArray[ meshIndex ] );

			for (auto &mesh : meshesToAdd)
			{
				btIndexedMesh part;
				auto &rawVertices = mesh->GetRawVertices( );
				auto &rawIndices = mesh->GetRawIndices( );

				part.m_vertexBase = reinterpret_cast<const unsigned char*>( &rawVertices[ 0 ] );
				part.m_vertexStride = sizeof( rawVertices[ 0 ] );
				part.m_numVertices = rawVertices.size( );

				part.m_triangleIndexBase = reinterpret_cast<const unsigned char*>( &rawIndices[ 0 ] );
				part.m_triangleIndexStride = 3 * sizeof( rawIndices[ 0 ] );
				part.m_numTriangles = rawIndices.size( ) / 3;
				part.m_indexType = PHY_INTEGER;

				m_triangleIndexVertexArray->addIndexedMesh( part );
			}

			m_triangleIndexVertexArray->calculateAabbBruteForce( m_localAabbMin, m_localAabbMax );

			buildOptimizedBvh( );

		#endif
		}

		void BvhTriangleMeshCollider::SetScale(const SVec3& scale)
		{
		#ifdef BULLET_PHYSICS

			setLocalScaling( scale.ToBullet( ) );

		#endif
		}

		void BvhTriangleMeshCollider::Serialize(Json::object& output) const
		{
		#ifdef BULLET_PHYSICS

			btDefaultSerializer serializer;

			// serialize( reinterpret_cast<void*>( this ), &serializer );

		#endif
		}

		void BvhTriangleMeshCollider::Deserialize(const Json& input)
		{
		#ifdef BULLET_PHYSICS



		#endif
		}
	}
}
