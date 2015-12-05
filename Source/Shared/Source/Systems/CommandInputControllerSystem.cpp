/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ButtonActionCommand.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "CommandInputControllerSystem.h"
#include "CommandInputControllerComponent.h"
#include <Components/CommandQueueComponent.h>
#include <SystemConfig.h>

ENTITY_SYSTEM_DEFINITION(CommandInputControllerSystem);

CommandInputControllerSystem::CommandInputControllerSystem(ursine::ecs::World* world) 
    : FilterSystem(world, ursine::ecs::Filter().All<CommandInputController, CommandQueue>(), 69) {}

void CommandInputControllerSystem::Process(ursine::ecs::Entity* entity)
{
    auto *commandInput = entity->GetComponent<CommandInputController>( );
    auto *commandQueue = entity->GetComponent<CommandQueue>( );

    auto &commandList = commandInput->GetCommandList( );

    for(auto &actionCommand : commandList)
    {
        if ( actionCommand->Acting() )
            commandQueue->AddCommand(actionCommand->CreateCommand());
    }
}