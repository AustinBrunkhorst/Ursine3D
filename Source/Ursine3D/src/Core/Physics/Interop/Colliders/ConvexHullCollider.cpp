/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ConvexHullCollider.cpp
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "UrsinePrecompiled.h"

#include "ConvexHullCollider.h"

#include "Model3DComponent.h"

#include <BulletCollision/CollisionShapes/btShapeHull.h>

namespace ursine
{
	namespace physics
	{
		ConvexHullCollider::ConvexHullCollider(void)
		{
			
		}

		void ConvexHullCollider::GenerateConvexHull(ecs::Model3D *model)
		{
		#ifdef BULLET_PHYSICS

			clearPoints( );

			// Add all points of the model to the convex hull
			auto modelResource = model->GetModelResource( );

			if (!modelResource)
			{
				UWarning("This model doesn't have a ModelResource.  Talk to Matt?");
				return;
			}

			auto meshesArray = model->GetModelResource( )->GetMeshArray( );
			auto meshIndex = model->GetMeshIndex( );
			std::vector<graphics::Mesh*> meshesToGenerate;

			if (meshIndex == -1)
				for (auto &mesh : meshesArray)
					meshesToGenerate.push_back( mesh );
			else
				meshesToGenerate.push_back( meshesArray[ meshIndex ] );

			for (auto &mesh : meshesToGenerate)
			{
				auto verts = mesh->GetRawVertices( );

				for (auto &vert : verts)
					addPoint( vert.ToBullet( ) );
			}

			// recalculate the AABB
			recalcLocalAabb( );

		#endif
		}

		void ConvexHullCollider::ReduceVertices(void)
		{
		#ifdef BULLET_PHYSICS

			//create a hull approximation
			btShapeHull hull( this );
			btScalar margin = getMargin( );

			hull.buildHull( margin );

			auto verts = hull.getVertexPointer( );
			auto count = hull.numVertices( );

			clearPoints( );

			for (int i = 0; i < count; ++i)
				addPoint( verts[ i ] );

			recalcLocalAabb( );

		#endif
		}

		float ConvexHullCollider::GetMargin(void) const
		{
		#ifdef BULLET_PHYSICS

			return getMargin( );

		#endif
		}

		void ConvexHullCollider::SetMargin(float margin)
		{
		#ifdef BULLET_PHYSICS

			setMargin( margin );

		#endif
		}

		void ConvexHullCollider::SetScale(const SVec3& scale)
		{
		#ifdef BULLET_PHYSICS

			m_localScaling = scale.ToBullet( );
			
			recalcLocalAabb( );

		#endif
		}

		void ConvexHullCollider::Serialize(Json::object& output) const
		{
		#ifdef BULLET_PHYSICS

			Json::array jsonVertArray;
			auto *vertArray = getUnscaledPoints( );
			auto vertCnt = getNumPoints( );

			for (int i = 0; i < vertCnt; ++i)
			{				
				Json::object elements;
				auto &vert = vertArray[ i ];

				elements[ "x" ] = vert.x( );
				elements[ "y" ] = vert.y( );
				elements[ "z" ] = vert.z( );

				jsonVertArray.push_back( Json( elements ) );
			}
			
			output[ "convexHullVerts" ] = jsonVertArray;

		#endif
		}

		void ConvexHullCollider::Deserialize(const Json& input)
		{
		#ifdef BULLET_PHYSICS

			auto convexHullVerts = input[ "convexHullVerts" ].array_items( );

			clearPoints( );

			for (auto &vert : convexHullVerts)
			{
				addPoint( btVector3(
					vert[ "x" ].number_value( ),
					vert[ "y" ].number_value( ),
					vert[ "z" ].number_value( )
				) );
			}

			recalcLocalAabb( );

		#endif
		}
	}
}
