/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EntityAnimatorDebugDrawSystem.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include <Precompiled.h>

#include "EntityAnimatorDebugDrawSystem.h"

#include "EntityAnimatorComponent.h"
#include <SelectedComponent.h>

using namespace ursine;
using namespace ecs;

ENTITY_SYSTEM_DEFINITION( EntityAnimatorDebugDrawSystem );

EntityAnimatorDebugDrawSystem::EntityAnimatorDebugDrawSystem(ursine::ecs::World *world)
    : FilterSystem( world, Filter( ).All<Selected, EntityAnimator>( ) )
{
    SetUpdateType( WORLD_EDITOR_UPDATE );
}

void EntityAnimatorDebugDrawSystem::Process(const EntityHandle &entity)
{
    auto animator = entity->GetComponent<EntityAnimator>( );

    animator->debugDraw( );
}
