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

#include "PlayerIdComponent.h"
#include "RevivePlayerComponent.h"
#include "HealthComponent.h"
#include "HitscanWeaponComponent.h"
#include "GameEvents.h"

#include <WorldEvent.h>
#include <CoreSystem.h>
#include <KeyboardManager.h>
#include <SystemManager.h>

using namespace ursine;
using namespace ecs;

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
    auto *kbManager = GetCoreSystem(KeyboardManager);

    if (kbManager->IsTriggeredDown(KEY_O))
    {
        // teleport player 2 to player 1
        auto players = m_world->GetEntitiesFromFilter( Filter( ).All<PlayerID>( ) );

        if (players.size( ) >= 2)
        {
            EntityHandle p1, p2;

            for (auto &player : players)
            {
                auto id = player->GetComponent<PlayerID>( )->GetID( );

                if (id == 0)
                    p1 = player;
                else if (id == 1)
                    p2 = player;
            }

            if (p1 && p2)
            {
                auto p2trans = p2->GetTransform( );
                auto p1trans = p1->GetTransform( );

                p2trans->SetWorldPosition(
                    p1trans->GetWorldPosition( ) + p1trans->GetRight( ) * 10.0f
                );

                p2trans->SetWorldRotation(
                    p1trans->GetWorldRotation( )
                );
            }
        }
    }
    if (kbManager->IsTriggeredDown(KEY_P))
    {
        // teleport player 1 to player 2
        auto players = m_world->GetEntitiesFromFilter( Filter( ).All<PlayerID>( ) );

        if (players.size( ) >= 2)
        {
            EntityHandle p1, p2;

            for (auto &player : players)
            {
                auto id = player->GetComponent<PlayerID>( )->GetID( );

                if (id == 0)
                    p1 = player;
                else if (id == 1)
                    p2 = player;
            }

            if (p1 && p2)
            {
                auto p2trans = p2->GetTransform( );
                auto p1trans = p1->GetTransform( );

                p1trans->SetWorldPosition( 
                    p2trans->GetWorldPosition( ) + p2trans->GetRight( ) * 10.0f
                );

                p1trans->SetWorldRotation(
                    p2trans->GetWorldRotation( )
                );
            }
        }
    }
    if (kbManager->IsTriggeredDown(KEY_I))
    {
        // revive players
        auto reviveObjects = m_world->GetEntitiesFromFilter( Filter( ).All<RevivePlayer>( ) );

        for (auto &reviveEntity : reviveObjects )
        {
            reviveEntity->Dispatch( game::REVIVE_PLAYER_CHEAT, EventArgs::Empty );
        }
    }
    if (kbManager->IsTriggeredDown(KEY_Y))
    {
        // Toggle immortality for both players
        auto players = m_world->GetEntitiesFromFilter( Filter( ).All<PlayerID>( ) );

        for (auto &player : players)
        {
            if (player->HasComponent<Health>( ))
            {
                auto health = player->GetComponent<Health>( );
                auto inv = health->GetInvulnerable( );
                health->SetInvulnerable( !inv );
            }
        }
    }
    if (kbManager->IsTriggeredDown(KEY_U))
    {
        // Toggle immortality for both players
        auto players = m_world->GetEntitiesFromFilter( Filter( ).All<PlayerID>( ) );

        for (auto &player : players)
        {
            auto weapon = player->GetComponentInChildren<HitscanWeapon>( );
            if (weapon)
            {
                weapon->ToggleMaxDamage( );
            }
        }
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