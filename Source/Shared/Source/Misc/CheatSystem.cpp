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

#include "InputControllerComponent.h"
#include "PlayerIdComponent.h"
#include "RevivePlayerComponent.h"
#include "HealthComponent.h"
#include "HitscanWeaponComponent.h"
#include "LevelSegmentManagerComponent.h"
#include "GameEvents.h"

#include <WorldEvent.h>
#include <CoreSystem.h>
#include <KeyboardManager.h>
#include <SystemManager.h>
#include <Scene.h>
#include <GameContext.h>

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

    skipSegmentCheat( kbManager );
}

void CheatSystem::skipSegmentCheat(KeyboardManager *kbManager)
{
    auto sm = m_world->GetEntitiesFromFilter( Filter( ).All<LevelSegmentManager>( ) );

    if (!sm.size( ))
        return;

    auto smManager = sm[ 0 ]->GetComponent<LevelSegmentManager>( );
    auto current = smManager->GetCurrentSegment( );
    auto desired = current;

    if (kbManager->IsTriggeredDown( KEY_1 ))
        desired = LevelSegments::Tut_OpeningCinematic;
    else if (kbManager->IsTriggeredDown( KEY_2 ))
        desired = LevelSegments::BossRoom_Platforming;
    else if (kbManager->IsTriggeredDown( KEY_3 ))
        desired = LevelSegments::BossRoom_Introduction;
    else if (kbManager->IsTriggeredDown( KEY_4 ))
        desired = LevelSegments::BossRoom_Phase1;
    else if (kbManager->IsTriggeredDown( KEY_5 ))
        desired = LevelSegments::BossRoom_Phase2;
    else if (kbManager->IsTriggeredDown( KEY_6 ))
        desired = LevelSegments::BossRoom_Phase3;
    else if (kbManager->IsTriggeredDown( KEY_7 ))
        desired = LevelSegments::BossRoom_Phase4;
    else if (kbManager->IsTriggeredDown( KEY_8 ))
        desired = LevelSegments::BossRoom_Phase5;
    else if (kbManager->IsTriggeredDown( KEY_9 ))
        desired = LevelSegments::EndingCredits;

    if (desired != current)
    {
        Application::Instance->PostMainThread( [=] {

            // create new world and set the new segment
            // by sending an event to the screen manager
            auto message = ui_event::global::GameSkip;
        
            auto data = Json::object( );
            data[ "Segment" ] = static_cast<int>( desired );
        
            UIScreenMessageArgs args( message, data );

            m_world->GetOwner( )->GetScreenManager( ).Dispatch( ui_event::global::GameSkip, &args );

        } );
    }
}
