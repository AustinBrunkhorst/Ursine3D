/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RandomSlerpSystem.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "RandomSlerpSystem.h"
#include <Components/RandomSlerpComponent.h>

ENTITY_SYSTEM_DEFINITION(RandomSlerpSystem);

RandomSlerpSystem::RandomSlerpSystem(ursine::ecs::World* world) 
    : FilterSystem(world, ursine::ecs::Filter().All<RandomSlerp>()) {}

void RandomSlerpSystem::Process(ursine::ecs::Entity* entity)
{
    entity->GetComponent<RandomSlerp>()->Update(0.016);
}