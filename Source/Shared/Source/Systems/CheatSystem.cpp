/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CheatSystem.cpp
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "CheatSystem.h"
#include <WorldEvent.h>
#include "SpawnSystem.h"
#include <Core/CoreSystem.h>
#include <Core/Input/Keyboard/KeyboardManager.h>
#include <Game Engine/Scene/World/Managers/SystemManager.h>

using namespace ursine;


ENTITY_SYSTEM_DEFINITION( CheatSystem );

CheatSystem::CheatSystem(ursine::ecs::World* world) : EntitySystem(world)
{
    return;
}

void CheatSystem::OnInitialize()
{
    m_world->Listener(this)
        .On(ecs::WorldEventType::WORLD_UPDATE, &CheatSystem::onUpdate);
}

void CheatSystem::OnRemove()
{
    m_world->Listener(this)
        .On(ecs::WorldEventType::WORLD_UPDATE, &CheatSystem::onUpdate);
}

void CheatSystem::onUpdate(EVENT_HANDLER(ursine::ecs:::World))
{
    int killCheck = killTeamCheat();
    if (killCheck > 0)
    {
        m_world->GetSystemManager()->GetSystem<SpawnSystem>()->DespawnTeam(killCheck);
    }
}

int CheatSystem::killTeamCheat(void)
{
    auto *kbManager = GetCoreSystem(KeyboardManager);

    if (kbManager->IsTriggeredDown(KEY_F9))
    {
        return 1;
    }
    if (kbManager->IsTriggeredDown(KEY_F10))
    {
        return 2;
    }

    return -1;
}