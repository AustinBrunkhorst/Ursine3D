/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PhysicsUtils.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "PhysicsUtils.h"

#include "Editor.h"
#include "SelectedComponent.h"

#include <ConvexHullColliderComponent.h>
#include <BvhTriangleMeshColliderComponent.h>
#include <Model3DComponent.h>
#include <FBXSceneRootNodeComponent.h>

#include <Timer.h>

using namespace ursine;
using namespace ecs;

static Entity *GetSelectedEntity(void)
{
	// find the selected entity
	auto world = GetCoreSystem( Editor )->GetProject( )->GetScene( )->GetWorld( );
	
	auto selected = world->GetEntitiesFromFilter( Filter( ).All<Selected>( ) );

	if (selected.size( ) > 0)
		return selected[ 0 ];
	else
		return nullptr;
}

JSFunction(GenerateColliderForModel)
{	
	return CefV8Value::CreateUndefined( );
}

JSFunction(ReduceConvexHull)
{
	return CefV8Value::CreateUndefined( );
}

JSFunction(GenerateCollidersForScene)
{
	auto entity = GetSelectedEntity( );

	if (entity)
	{
		auto sceneRoot = entity->GetComponent<FBXSceneRootNode>( );

		if (!sceneRoot)
			// TODO: Send toast notification
			return CefV8Value::CreateUndefined( );

		auto models = entity->GetComponentsInChildren<Model3D>( );

		for (auto &model : models)
		{
			auto entity = model->GetOwner( );

			if (!entity->HasComponent<ConvexHullCollider>( ))
				entity->AddComponent<ConvexHullCollider>( );

			auto convexHull = entity->GetComponent<ConvexHullCollider>( );

			convexHull->GenerateConvexHull( model );
		}
	}

	return CefV8Value::CreateUndefined( );
}

JSFunction(GenerateBvhTriangleColliderForModel)
{
	auto entity = GetSelectedEntity( );

	if (entity)
	{
		auto model = entity->GetComponent<Model3D>( );

		// if (!model)
			// TODO: Send toast notification to user that you can't generate collider without model

		Timer::Create(0).Completed([=] {
			if (!entity->HasComponent<BvhTriangleMeshCollider>( ))
				entity->AddComponent<BvhTriangleMeshCollider>( );

			auto bvhTriangleMesh = entity->GetComponent<BvhTriangleMeshCollider>( );

			bvhTriangleMesh->GenerateBvhTriangleMesh( model );
		} );
	}
	else
	{
		// TODO: Send toast notification to user that they should select an entity first
	}
	
	return CefV8Value::CreateUndefined( );
}

JSFunction(GenerateBvhTriangleColliderForScene)
{
	auto entity = GetSelectedEntity( );

	if (entity)
	{
		auto sceneRoot = entity->GetComponent<FBXSceneRootNode>( );

		if (!sceneRoot)
			// TODO: Send toast notification
			return CefV8Value::CreateUndefined( );

		auto models = entity->GetComponentsInChildren<Model3D>( );

		Timer::Create( 0 ).Completed( [=] {
			for (auto &model : models)
			{
				auto entity = model->GetOwner( );

				if (!entity->HasComponent<BvhTriangleMeshCollider>( ))
					entity->AddComponent<BvhTriangleMeshCollider>( );

				auto bvhTriangleMesh = entity->GetComponent<BvhTriangleMeshCollider>( );

				bvhTriangleMesh->GenerateBvhTriangleMesh( model );
			}
		} );
	}

	return CefV8Value::CreateUndefined( );
}
