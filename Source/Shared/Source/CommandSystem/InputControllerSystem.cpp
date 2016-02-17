/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ButtonActionCommand.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "InputControllerSystem.h"
#include "InputControllerComponent.h"
#include <CommandQueueComponent.h>
#include <SystemConfig.h>

ENTITY_SYSTEM_DEFINITION(InputControllerSystem);

using namespace ursine;
using namespace ursine::ecs;

InputControllerSystem::InputControllerSystem(ursine::ecs::World* world) 
    : FilterSystem( world, Filter( ).All<InputController, CommandQueue>( ) )
{

}

void InputControllerSystem::Process(ursine::ecs::Entity* entity)
{
    auto *commandInput = entity->GetComponent<InputController>( );
    auto *commandQueue = entity->GetComponent<CommandQueue>( );

    auto &commandList = commandInput->GetCommandList( );

    for(auto &actionCommand : commandList)
    {
        if ( actionCommand->Acting() )
            commandQueue->AddCommand(actionCommand->CreateCommand());
    }
}
