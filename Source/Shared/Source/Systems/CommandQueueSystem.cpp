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
#include <Components/CommandQueueComponent.h>

#include <SystemConfig.h>

ENTITY_SYSTEM_DEFINITION(CommandQueueSystem);

CommandQueueSystem::CommandQueueSystem(ursine::ecs::World* world)
	: FilterSystem(world, ursine::ecs::Filter().All<CommandQueue>(), 68) {}

void CommandQueueSystem::Process(ursine::ecs::Entity* entity)
{
	auto *commandQueue = entity->GetComponent<CommandQueue>();
	commandQueue->Update();
}

