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

#include <Model3DComponent.h>

using namespace ursine;
using namespace ecs;

JSFunction(GenerateColliders)
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

		auto meshes = model->GetModelResource( )->GetMeshArray( );

		for (auto &mesh : meshes)
		{
			// pass the physics component the mesh 
			// (meaning we can have multiple colliders to one body?) 
			// (Option 2, just have a convex hull component) 
			// (Option 3 multiple convex hull components, that hvae the name of the mesh they represent, that way you can remove ones you don't want "i.e. fanny pack")
		}
	}
	else
	{
		// TODO: Send toast notification to user that they should select an entity first
	}


	// get the mesh

	// generate a collider for the mesh's vertices
	
	return CefV8Value::CreateUndefined( );
}
