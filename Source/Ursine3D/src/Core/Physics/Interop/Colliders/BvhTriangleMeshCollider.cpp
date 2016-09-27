/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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

#include "base91.hpp"

#include "BulletWorldImporter/btBulletWorldImporter.h"

namespace ursine
{
    namespace physics
    {
    #ifdef BULLET_PHYSICS

        BvhTriangleMeshCollider::BvhTriangleMeshCollider(void)
            : BvhTriangleMeshColliderBase( 
                new btBvhTriangleMeshShape( new btTriangleIndexVertexArray( ), true, false ), btVector3(1.0f, 1.0f, 1.0f)
            )
        {
            m_triangleIndexVertexArray = reinterpret_cast<btTriangleIndexVertexArray*>( getChildShape( )->getMeshInterface( ) );

            // Turn off debug drawing at first (until the mesh is generated)
            m_shapeType = INVALID_SHAPE_PROXYTYPE;
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
            std::vector<graphics::MeshResource*> meshesToAdd;

            if (meshIndex == -1)
                for (auto &mesh : meshArray)
                    meshesToAdd.push_back( mesh );
            else
                meshesToAdd.push_back( meshArray[ meshIndex ] );

            for (auto &mesh : meshesToAdd)
            {
                btIndexedMesh part;
                auto &rawVertices = mesh->GetVertexArray( );
                auto &rawIndices = mesh->GetIndexArray( );

                part.m_vertexBase = reinterpret_cast<const unsigned char*>( &rawVertices[ 0 ] );
                part.m_vertexStride = sizeof( rawVertices[ 0 ] );
                part.m_numVertices = static_cast<int>( rawVertices.size( ) );

                part.m_triangleIndexBase = reinterpret_cast<const unsigned char*>( &rawIndices[ 0 ] );
                part.m_triangleIndexStride = 3 * sizeof( rawIndices[ 0 ] );
                part.m_numTriangles = static_cast<int>( rawIndices.size( ) / 3u );
                part.m_indexType = PHY_INTEGER;

                m_triangleIndexVertexArray->addIndexedMesh( part );
            }

            m_triangleIndexVertexArray->calculateAabbBruteForce( 
                const_cast<btVector3&>( getChildShape( )->getLocalAabbMin( ) ),
                const_cast<btVector3&>( getChildShape( )->getLocalAabbMax( ) ) 
            );

            getChildShape( )->buildOptimizedBvh( );

            m_shapeType = SCALED_TRIANGLE_MESH_SHAPE_PROXYTYPE;

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
            
            serializer.startSerialization( );

            getChildShape( )->serializeSingleShape( &serializer );

            serializer.finishSerialization( );

            auto buffer = serializer.getBufferPointer( );
            auto size = serializer.getCurrentBufferSize( );

            auto encoded_91 = base91::encode( std::string( 
                reinterpret_cast<const char *>( buffer ), size 
            ) );

            output[ "data" ] = encoded_91;

        #endif
        }

        void BvhTriangleMeshCollider::Deserialize(const Json& input)
        {
        #ifdef BULLET_PHYSICS

            auto encoded91 = input[ "data" ].string_value( );
            auto decoded91 = base91::decode( encoded91 );

            btBulletWorldImporter import;

            import.loadFileFromMemory( 
                reinterpret_cast<char*>( &decoded91[ 0 ] ), 
                static_cast<int>( decoded91.size( ) )
            );

            auto numShapes = import.getNumCollisionShapes( );

            UAssert( numShapes > 0, 
                "Something went wrong when Serializeing the BvhTriangleMeshCollider." 
            );

            setChildShape( 
                reinterpret_cast<btBvhTriangleMeshShape*>( import.getCollisionShapeByIndex( 0 ) ) 
            );

            m_shapeType = SCALED_TRIANGLE_MESH_SHAPE_PROXYTYPE;

        #endif
        }
    }
}
