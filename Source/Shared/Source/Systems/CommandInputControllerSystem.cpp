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
    : EntitySystem( world )
{
    
}

void CommandInputControllerSystem::OnInitialize(void)
{
    m_world->Listener( this )
        .On( WORLD_ENTITY_COMPONENT_ADDED, &CommandInputControllerSystem::onComponentAdded )
        .On( WORLD_ENTITY_COMPONENT_REMOVED, &CommandInputControllerSystem::onComponentRemoved )
        .On( WORLD_UPDATE, &CommandInputControllerSystem::onUpdate );
}

void CommandInputControllerSystem::OnRemove(void)
{
    m_world->Listener( this )
        .Off( WORLD_ENTITY_COMPONENT_ADDED, &CommandInputControllerSystem::onComponentAdded )
        .Off( WORLD_ENTITY_COMPONENT_REMOVED, &CommandInputControllerSystem::onComponentRemoved )
        .Off( WORLD_UPDATE, &CommandInputControllerSystem::onUpdate );
}

void CommandInputControllerSystem::onComponentAdded(EVENT_HANDLER(World))
{
    EVENT_ATTRS(World, ComponentEventArgs);
    
    auto component = args->component;
    auto entity = args->entity;

    if (component->Is<CommandInputController>( ) && entity->HasComponent<CommandQueue>( ))
        m_entities.push_back( entity );
    else if (component->Is<CommandQueue>( ) && entity->HasComponent<CommandInputController>( ))
        m_entities.push_back( entity );
}

void CommandInputControllerSystem::onComponentRemoved(EVENT_HANDLER(World))
{
    EVENT_ATTRS(World, ComponentEventArgs);

    auto component = args->component;
    auto entity = args->entity;

    if (component->Is<CommandInputController>( ) && entity->HasComponent<CommandQueue>( ))
        m_entities.erase( std::find( m_entities.begin( ), m_entities.end( ), entity ) );
    else if (component->Is<CommandQueue>( ) && entity->HasComponent<CommandInputController>( ))
        m_entities.erase( std::find( m_entities.begin( ), m_entities.end( ), entity ) );
}

void CommandInputControllerSystem::onUpdate(EVENT_HANDLER(World))
{
    for (auto e : m_entities)
        Process( e );
}

void CommandInputControllerSystem::Process(ursine::ecs::Entity* entity)
{
    auto *commandInput = entity->GetComponent<CommandInputController>( );
    auto *commandQueue = entity->GetComponent<CommandQueue>( );

    URSINE_TODO("THIS IS A HACK, REMOVE IT");
    auto *playerInput = entity->GetComponent<PlayerInput>( );

    if (true)
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
