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
#include <Model3DComponent.h>
#include <FBXSceneRootNodeComponent.h>

using namespace ursine;
using namespace ecs;

JSFunction(GenerateColliderForModel)
{
	// find the selected entity
	auto world = GetCoreSystem( Editor )->GetProject( )->GetScene( )->GetWorld( );
	
	auto selected = world->GetEntitiesFromFilter( Filter( ).All<Selected>( ) );

	if (selected.size( ) > 0)
	{
		auto entity = selected[ 0 ];
		auto model = entity->GetComponent<Model3D>( );

		// if (!model)
			// TODO: Send toast notification to user that you can't generate collider without model

		if (!entity->HasComponent<ConvexHullCollider>( ))
			entity->AddComponent<ConvexHullCollider>( );

		auto convexHull = entity->GetComponent<ConvexHullCollider>( );

		convexHull->GenerateConvexHull( model );
	}
	else
	{
		// TODO: Send toast notification to user that they should select an entity first
	}
	
	return CefV8Value::CreateUndefined( );
}

JSFunction(ReduceConvexHull)
{
	// find the selected entity
	auto world = GetCoreSystem( Editor )->GetProject( )->GetScene( )->GetWorld( );

	auto selected = world->GetEntitiesFromFilter( Filter( ).All<Selected>( ) );

	if (selected.size( ) > 0)
	{
		auto entity = selected[ 0 ];
		auto convexHull = entity->GetComponent<ConvexHullCollider>( );

		if (!convexHull)
			// TODO: Send toast notification
			return CefV8Value::CreateUndefined( );

		convexHull->ReduceVertices( );
	}

	return CefV8Value::CreateUndefined( );
}

JSFunction(GenerateCollidersForScene)
{
	// find the selected entity
	auto world = GetCoreSystem( Editor )->GetProject( )->GetScene( )->GetWorld( );

	auto selected = world->GetEntitiesFromFilter( Filter( ).All<Selected>( ) );

	if (selected.size( ) > 0)
	{
		auto selectedEntity = selected[ 0 ];
		auto sceneRoot = selectedEntity->GetComponent<FBXSceneRootNode>( );

		if (!sceneRoot)
			// TODO: Send toast notification
			return CefV8Value::CreateUndefined( );

		auto models = selectedEntity->GetComponentsInChildren<Model3D>( );

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
