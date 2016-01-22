/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ConvexDecompCollider.cpp
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "ConvexDecompCollider.h"
#include "Body.h"
#include "Rigidbody.h"

#include "Model3DComponent.h"

#include <HACD/hacdHACD.h>

namespace ursine
{
	namespace physics
	{
		ConvexDecompCollider::ConvexDecompCollider(void)
		{
		}

		ConvexDecompCollider::~ConvexDecompCollider(void)
		{
			clearConvexHulls( );
		}

		void ConvexDecompCollider::GenerateConvexHulls(ecs::Model3D* model)
		{
			clearConvexHulls( );

		#ifdef BULLET_PHYSICS
			
			auto modelResource = model->GetModelResource( );

			if (!modelResource)
			{
				UWarning("This model doesn't have a ModelResource.  Talk to Matt?");
				return;
			}

			auto meshesArray = modelResource->GetMeshArray( );
			auto meshIndex = model->GetMeshIndex( );
			std::vector<graphics::Mesh*> meshesToDecompose;

			if (meshIndex == -1)
				for (auto &mesh : meshesArray)
					meshesToDecompose.push_back( mesh );
			else
				meshesToDecompose.push_back( meshesArray[ meshIndex ] );

			// Process all meshes of the model
			for (auto &mesh : meshesToDecompose)
			{
				// Get the points and triangles for the hacd construction
				std::vector< HACD::Vec3<HACD::Real> > points;
				std::vector< HACD::Vec3<long> > triangles;

				for (auto &vert : mesh->GetRawVertices( ))
				{
					points.push_back( {
						vert.X( ), vert.Y( ), vert.Z( )
					} );
				}

				auto &indices = mesh->GetRawIndices( );

				for (int i = 0, n = indices.size( ); i < n; i += 3)
				{
					triangles.push_back( {
						static_cast<int>( indices[ i ] ),
						static_cast<int>( indices[ i + 1 ] ),
						static_cast<int>( indices[ i + 2 ] )
					} );
				}

				// Here is where the convex decomposition starts
				HACD::HACD hacd;

				hacd.SetPoints( &points[ 0 ] );
				hacd.SetNPoints( points.size( ) );
				hacd.SetTriangles( &triangles[ 0 ] );
				hacd.SetNTriangles( triangles.size( ) );
				hacd.SetCompacityWeight( 0.1 );
				hacd.SetVolumeWeight( 0.0f );

				// HACD parameters
				// Recommended parameters (from bullet's example):
				// 2 100 100 0 0 0 0
				hacd.SetNClusters( 1 );        // minimum number of clusters
				hacd.SetNVerticesPerCH( 100 ); // max of 100 vertices per convex-hull
				hacd.SetConcavity( 100 );      // maximum concavity
				hacd.SetAddExtraDistPoints( false );
				hacd.SetAddNeighboursDistPoints( false );
				hacd.SetAddFacesPoints( false );

				// Compute the new convex hull clusters
				hacd.Compute( );
				
				// take all these shapes (clusters) and add them to a compound shape
				auto clusterNum = hacd.GetNClusters( );

				for (auto i = 0; i < clusterNum; ++i)
				{
					std::vector< HACD::Vec3< HACD::Real > > pointsCH( hacd.GetNPointsCH( i ) );
					std::vector< HACD::Vec3< long > > trianglesCH( hacd.GetNTrianglesCH( i ) );

					// Get this cluster's points and triangles
					hacd.GetCH( i, &pointsCH[ 0 ], &trianglesCH[ 0 ] );

					// Create a new convexHullCollider
					auto newConvexHull = new ConvexHullCollider;
					btVector3 centroid = btVector3( 0, 0, 0 );

					// Add the points and calculate the centroid
					for (auto &p : pointsCH)
					{
						auto point = btVector3( p.X( ), p.Y( ), p.Z( ) );

						centroid += point;
					}

					centroid *= 1.0f / pointsCH.size( );

					for (auto &p : pointsCH)
					{
						auto point = btVector3( p.X( ), p.Y( ), p.Z( ) );

						point -= centroid;

						newConvexHull->addPoint( point );
					}

					newConvexHull->recalcLocalAabb( );
					newConvexHull->ToggleDebugDraw( true );

					// add the shape to the compound shape
					btTransform trans;

					trans.setIdentity( );
					trans.setOrigin( centroid );

					addChildShape( trans, newConvexHull );
				}
			}

		#endif
		}

		void ConvexDecompCollider::Serialize(Json::object& output) const
		{
		#ifdef BULLET_PHYSICS

			Json::array convexHulls;
			Json::array centroids;

			// serialize all "ConvexHullColliders"
			auto num = getNumChildShapes( );

			for (int i = 0; i < num; ++i)
			{
				auto *shape = getChildShape( i );
				auto *convexHull = reinterpret_cast<const ConvexHullCollider*>( shape );
				
				Json::object obj;

				convexHull->Serialize( obj );
				convexHulls.push_back( obj );

				auto centroid = getChildTransform( i ).getOrigin( );
				centroids.push_back( 
					meta::Variant( SVec3( centroid ) 
				).SerializeJson( ) );
			}

			output[ "convexHulls" ] = convexHulls;
			output[ "centroids" ] = centroids;

		#endif
		}

		void ConvexDecompCollider::Deserialize(const Json& input)
		{
		#ifdef BULLET_PHYSICS

			clearConvexHulls( );

			auto type = typeof(SVec3);

			auto &convexHulls = input[ "convexHulls" ].array_items( );
			auto &centroids = input[ "centroids" ].array_items( );

			for (int i = 0; i < convexHulls.size( ); ++i)
			{
				auto child = new ConvexHullCollider;

				child->Deserialize( convexHulls[ i ] );

				auto centroid = type.DeserializeJson( centroids[ i ] );

				// add the shape to the compound shape
				btTransform trans;

				trans.setIdentity( );
				trans.setOrigin( centroid.GetValue<SVec3>( ).ToBullet( ) );

				addChildShape( trans, child );
			}

		#endif
		}

		void ConvexDecompCollider::clearConvexHulls(void)
		{
		#ifdef BULLET_PHYSICS

			while (getNumChildShapes( ) > 0)
			{
				delete getChildShape( 0 );
				removeChildShapeByIndex( 0 );
			}

		#endif
		}

		void ConvexDecompCollider::SetScale(const SVec3& scale)
		{
		#ifdef BULLET_PHYSICS

			setLocalScaling( scale.ToBullet( ) );

		#endif
		}

		float ConvexDecompCollider::GetMargin(void) const
		{
		#ifdef BULLET_PHYSICS

			return getMargin( );

		#endif
		}

		void ConvexDecompCollider::SetMargin(float margin)
		{
		#ifdef BULLET_PHYSICS

			setMargin( margin );

		#endif
		}
	}
}