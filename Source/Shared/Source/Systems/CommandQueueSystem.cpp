/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CommandQueueSystem.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "CommandQueueSystem.h"

#include <SystemConfig.h>

ENTITY_SYSTEM_DEFINITION(CommandQueueSystem);

CommandQueueSystem::CommandQueueSystem(ursine::ecs::World* world) 
    : EntitySystem(world) {}

void CommandQueueSystem::OnInitialize()
{
    m_world->Listener(this)
        .On(ursine::ecs::WorldEventType::WORLD_ENTITY_COMPONENT_ADDED
            , &CommandQueueSystem::onComponentAdded)
        .On(ursine::ecs::WorldEventType::WORLD_ENTITY_COMPONENT_REMOVED
            , &CommandQueueSystem::onComponentRemoved)
        .On(ursine::ecs::WorldEventType::WORLD_UPDATE, &CommandQueueSystem::onWorldUpdate);
}

void CommandQueueSystem::OnRemove()
{
    m_world->Listener(this)
        .Off(ursine::ecs::WorldEventType::WORLD_ENTITY_COMPONENT_ADDED
            , &CommandQueueSystem::onComponentAdded)
        .Off(ursine::ecs::WorldEventType::WORLD_ENTITY_COMPONENT_REMOVED
            , &CommandQueueSystem::onComponentRemoved)
        .Off(ursine::ecs::WorldEventType::WORLD_UPDATE, &CommandQueueSystem::onWorldUpdate);
}

void CommandQueueSystem::onComponentAdded(EVENT_HANDLER(ursine::ecs:::World))
{
    EVENT_ATTRS(ursine::ecs::World, ursine::ecs::ComponentEventArgs);

    if (args->component->Is<CommandQueue>( ))
    {
        m_commandQueueList.push_front(reinterpret_cast<CommandQueue *>( args->component ) );
    }
}

void CommandQueueSystem::onComponentRemoved(EVENT_HANDLER(ursine::ecs:::World))
{
    EVENT_ATTRS(ursine::ecs::World, ursine::ecs::ComponentEventArgs);

    if (args->component->Is<CommandQueue>( ))
    {
        m_commandQueueList.remove(reinterpret_cast<CommandQueue *>( args->component ) );
    }
}

void CommandQueueSystem::onWorldUpdate(EVENT_HANDLER(ursine::ecs:::World))
{
    for (auto comQueue : m_commandQueueList)
    {
        Process(comQueue);
    }
}

void CommandQueueSystem::Process(CommandQueue* commandQueue)
{
    commandQueue->Update();
}



