/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ClothDebugDrawSystem.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include <SelectedComponent.h>

using namespace ursine;
using namespace ecs;

ENTITY_SYSTEM_DEFINITION( ClothDebugDrawSystem );

ClothDebugDrawSystem::ClothDebugDrawSystem(ursine::ecs::World* world)
    : FilterSystem( world, Filter( ).All<Selected, Cloth>( ) )
{
    SetUpdateType( WORLD_EDITOR_UPDATE );
}

void ClothDebugDrawSystem::Process(const EntityHandle &entity)
{
    auto cloth = entity->GetComponent<Cloth>( );

    if (cloth->GetDebugDraw( ))
    {
        cloth->DebugDraw( );
    }
}
