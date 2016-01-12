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
#include <Components/PlayerInputComponent.h>
#include <Core/CoreSystem.h>
#include <Core/Input/Keyboard/KeyboardManager.h>
#include <CommandSystem/AxisCommandTypes/MoveCommand.h>
#include <Core/Input/Mouse/MouseManager.h>
#include <CommandSystem/CommandTypes/FireCommand.h>
#include <CommandSystem/AxisCommandTypes/LookCommand.h>

ENTITY_SYSTEM_DEFINITION(CommandInputControllerSystem);

using namespace ursine;
using namespace ursine::ecs;

CommandInputControllerSystem::CommandInputControllerSystem(ursine::ecs::World* world) 
    : FilterSystem( world, Filter( ).All<CommandInputController, CommandQueue>( ) )
{
    
}

void CommandInputControllerSystem::Process(ursine::ecs::Entity* entity)
{
    auto *commandInput = entity->GetComponent<CommandInputController>( );
    auto *commandQueue = entity->GetComponent<CommandQueue>( );

    URSINE_TODO("THIS IS A HACK, REMOVE IT");
    auto *playerInput = entity->GetComponent<PlayerInput>( );

    if (playerInput && playerInput->keyboard)
    {
        auto keyboard = GetCoreSystem( KeyboardManager );

        Vec2 moveDir;

        if (keyboard->IsDown( KEY_W ))
            moveDir.Y( ) += 1;
        if (keyboard->IsDown( KEY_S ))
            moveDir.Y( ) -= 1;
        if (keyboard->IsDown( KEY_A ))
            moveDir.X( ) -= 1;
        if (keyboard->IsDown( KEY_D ))
            moveDir.X( ) += 1;

        if (moveDir != Vec2::Zero( ))
            commandQueue->AddCommand( std::make_shared<MoveCommand>( moveDir ) );

        if (keyboard->IsTriggeredDown( KEY_SPACE ))
            commandQueue->AddCommand( std::make_shared<JumpCommand>( ) );

        auto mouse = GetCoreSystem( MouseManager );

        if (mouse->IsButtonDown( MBTN_LEFT ))
            commandQueue->AddCommand( std::make_shared<FireCommand>( ) );

        Vec2 mouseMove = mouse->GetPositionDelta( ) * 0.5f;

        mouseMove.X( ) = -mouseMove.X( );

        if (mouseMove != Vec2::Zero( ))
            commandQueue->AddCommand( std::make_shared<LookCommand>( mouseMove ) );

        return;
    }

    auto &commandList = commandInput->GetCommandList( );

    for(auto &actionCommand : commandList)
    {
        if ( actionCommand->Acting() )
            commandQueue->AddCommand(actionCommand->CreateCommand());
    }
}
