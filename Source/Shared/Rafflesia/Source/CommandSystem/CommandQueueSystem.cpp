/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CommandQueueSystem.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "CommandQueueSystem.h"
#include <CommandQueueComponent.h>
#include <InputControllerComponent.h>

#include <SystemConfig.h>

ENTITY_SYSTEM_DEFINITION( CommandQueueSystem );

CommandQueueSystem::CommandQueueSystem(ursine::ecs::World *world)
    : FilterSystem( world, ursine::ecs::Filter( ).All<CommandQueue, InputController>( ), 68 ) { }

void CommandQueueSystem::Process(const ursine::ecs::EntityHandle &entity)
{
    auto *commandQueue = entity->GetComponent<CommandQueue>( );

    UAssert( commandQueue != nullptr, "HEY THERE" );

    commandQueue->Update( );
}

