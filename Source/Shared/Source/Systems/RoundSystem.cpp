#include "Precompiled.h"

#include "RoundSystem.h"
#include <WorldEvent.h>
#include "SpawnSystem.h"
#include <Core/CoreSystem.h>
#include <Core/Input/Keyboard/KeyboardManager.h>
#include <Game Engine/Scene/World/Managers/SystemManager.h>

using namespace ursine;


ENTITY_SYSTEM_DEFINITION( RoundSystem );

RoundSystem::RoundSystem(ursine::ecs::World* world) : EntitySystem(world)
{
    return;
}
