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

    auto &commandList = commandInput->GetCommandList( );

    for(auto &actionCommand : commandList)
    {
        if ( actionCommand->Acting() )
            commandQueue->AddCommand(actionCommand->CreateCommand());
    }
}
