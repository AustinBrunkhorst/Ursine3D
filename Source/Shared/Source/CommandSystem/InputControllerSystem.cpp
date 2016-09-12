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
#include <GamepadManager.h>

ENTITY_SYSTEM_DEFINITION(InputControllerSystem);

using namespace ursine;
using namespace ursine::ecs;

InputControllerSystem::InputControllerSystem(ursine::ecs::World* world) 
    : FilterSystem( world, Filter( ).All<InputController, CommandQueue>( ) )
{
}

void InputControllerSystem::Process(const ursine::ecs::EntityHandle &entity)
{
    auto *inputController = entity->GetComponent<InputController>();

    auto player = entity->GetComponent<PlayerID>( );

    if (player->GetID( ) == 0)
    {
        // check to see if there aren't any controllers connected
        auto connected = GetCoreSystem( GamepadManager )->NumConnected( );

        if (connected == 0 && !inputController->GetKeyBoard( ))
            inputController->SetKeyBoard( true );
        else if (connected != 0 && inputController->GetKeyBoard( ))
            inputController->SetKeyBoard( false );
    }

    auto &actionList = inputController->GetActionList( );

    for(auto &actionCommand : actionList)
    {
        actionCommand->ProcessCommands( );
    }
}
