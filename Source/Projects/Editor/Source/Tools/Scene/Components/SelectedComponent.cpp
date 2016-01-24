/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SelectedComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "SelectedComponent.h"

#include <Model3DComponent.h>

using namespace ursine;

NATIVE_COMPONENT_DEFINITION( Selected );

Selected::Selected(void)
    : BaseComponent( ) { }

Selected::~Selected(void)
{
    tryDebugModel( false );

	auto owner = GetOwner( );

	owner->GetWorld( )->Listener( this )
		.Off( ecs::WORLD_ENTITY_COMPONENT_ADDED, &Selected::onComponentAdded);
}

void Selected::OnInitialize(void)
{
    auto owner = GetOwner( );

	owner->GetWorld( )->Listener( this )
		.On( ecs::WORLD_ENTITY_COMPONENT_ADDED, &Selected::onComponentAdded);

    tryDebugModel( true );
}

void Selected::tryDebugModel(bool enabled)
{
    auto model = GetOwner( )->GetComponent<ecs::Model3D>( );

    // nope the fuck out of here
    if (!model)
        return;

    model->SetDebug( enabled );
}

void Selected::onComponentAdded(EVENT_HANDLER(ecs::World))
{
	EVENT_ATTRS(ecs::World, ecs::ComponentEventArgs);
	
	if (args->component->Is<ecs::Model3D>( ))
		tryDebugModel( true );
}
