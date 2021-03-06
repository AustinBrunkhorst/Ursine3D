/* ----------------------------------------------------------------------------
** Team Bear King
** � 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerLookAtSystem.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#include "Precompiled.h"
#include "PlayerLookAtSystem.h"
#include "Application.h"
#include "RaycastComponent.h"
#include "PlayerLookAtComponent.h"
#include "PlayerIdComponent.h"
#include "HealthComponent.h"
#include "UIEvents.h"
#include "EntityEvent.h"

#include <Scene.h>

ENTITY_SYSTEM_DEFINITION( PlayerLookAtSystem );

using namespace ursine;
using namespace ecs;

namespace
{
    void SendReticleActiveEvent(UIScreensConfig* ui, int id, bool state)
    {
        if ( ui )
        {
            // crete reticle event
            ui_event::ReticleActive reticleEvent;
            reticleEvent.playerID = id;
            reticleEvent.active = state;

            ui->TriggerPlayerHUDEvent(reticleEvent);
        }
    }

    void SendHealthTrackStartEvent(UIScreensConfig* ui, int id, const std::string& dispName, float healthPercent)
    {
        if ( ui )
        {
            // create track event for ui
            ui_event::HealthTrackStart trackEvent;

            trackEvent.playerID = id;
            trackEvent.enemyName = dispName;
            trackEvent.healthPercent = healthPercent;

            ui->TriggerPlayerHUDEvent(trackEvent);
        }
    }

    void SendHealthTrackUpdateEvent(UIScreensConfig* ui, int id, float healthPercent)
    {
        if ( ui )
        {
            // create track update event
            ui_event::HealthTrackUpdate trackEvent;

            trackEvent.playerID = id;
            trackEvent.healthPercent = healthPercent;

            ui->TriggerPlayerHUDEvent(trackEvent);
        }
    }

    void SendHealthTrackEndEvent(UIScreensConfig* ui, int id)
    {
        if ( ui )
        {
            // create track end event
            ui_event::HealthTrackEnd trackEvent;

            trackEvent.playerID = id;
            trackEvent.enemyKilled = false;

            ui->TriggerPlayerHUDEvent(trackEvent);
        }
    }

} // unnamed namespace


//////////////////////////////
////  Base Weapon System  ////
//////////////////////////////

PlayerLookAtSystem::PlayerLookAtSystem(World *world)
    : FilterSystem( world, Filter( ).All<PlayerLookAt, RaycastComponent, PlayerID>( ) )
    , m_ui( nullptr )
{
}


void PlayerLookAtSystem::Enable(const EntityHandle &entity)
{
    m_playerLookAtComps[ entity ] = entity->GetComponent<PlayerLookAt>( );

    m_playerIDs[ entity ] = entity->GetComponent<PlayerID>( )->GetID( );

    m_raycastComps[ entity ] = entity->GetComponent<RaycastComponent>( );
}

void PlayerLookAtSystem::Disable(const EntityHandle &entity)
{
    m_playerLookAtComps.erase( entity );
    m_playerIDs.erase( entity );
    m_raycastComps.erase( entity );
}

void PlayerLookAtSystem::OnSceneReady(Scene *scene)
{
    auto manager = scene->GetGameContext( )->GetManager( );

    if (!manager)
        return;

    m_ui = manager->GetConfigComponent<UIScreensConfig>( );
}

void PlayerLookAtSystem::onUpdate(EVENT_HANDLER(World))
{
    float dt = Application::Instance->GetDeltaTime( );

    RaycastComponent *rayComp;
    PlayerLookAt *playerLookComp;
    Health *enemyHealthComp;

    EntityHandle enemyHandle;

    int id;

    for (auto playerLookIt : m_playerLookAtComps)
    {
        rayComp = m_raycastComps[ playerLookIt.first ];

        playerLookComp = playerLookIt.second;

        id = m_playerIDs[ playerLookIt.first ];

        enemyHandle = rayComp->GetEntityHit( );

        // are we lookin at enemy
        if (enemyHandle != nullptr)
        {
            // is health comp on entity
            enemyHealthComp = enemyHandle->GetComponent<Health>( );

            // if not on entity then on root
            if (enemyHealthComp == nullptr)
                enemyHealthComp = enemyHandle->GetRoot( )->GetComponent<Health>( );

            UAssert( enemyHealthComp, "Error: The raycast component requires there to be a health component." );

            if (enemyHealthComp->GetHealth( ) <= 0.0f)
                continue;

            // calculate enemy health percentage
            float enemyHealthPercentage = enemyHealthComp->GetHealth( ) / enemyHealthComp->GetMaxHealth( );

            // new obj
            if (!playerLookComp->ReticleActive( ) || playerLookComp->GetCurrentEnemy( ) != enemyHandle)
            {
                auto &currEnemyHealth = playerLookComp->GetCurrentEnemyHealth( );

                // unsubsribe to old enemies death
                if (currEnemyHealth != nullptr)
                {
                    currEnemyHealth->Listener( playerLookComp )
                        .Off( HealthEvents::HEALTH_ZERO, &PlayerLookAt::onEnemyDeath );
                }

                // change curr enemy
                playerLookComp->SetCurrentEnemy( enemyHandle );
                playerLookComp->SetCurrentEnemyHealth( enemyHealthComp );
                playerLookComp->SetReticleActive( true );
                playerLookComp->SetHealthPercent( enemyHealthPercentage );

                // send ui events
                SendHealthTrackStartEvent( m_ui, id, enemyHealthComp->GetDisplayName( ), enemyHealthPercentage );
                SendReticleActiveEvent( m_ui, id, true );

                // connect to enemyhit death event
                enemyHealthComp->Listener(playerLookComp)
                    .On( HealthEvents::HEALTH_ZERO, &PlayerLookAt::onEnemyDeath );
            }

            // not new
            else if (enemyHealthPercentage != playerLookComp->GetHealthPercent( ))
            {
                SendHealthTrackUpdateEvent( m_ui, id, enemyHealthPercentage );
            }

            // set timer to 0.0f
            playerLookComp->SetTimer( 0.0f );
        }

        // not looking at enemy
        else if (playerLookComp->ReticleActive( ))
        {
            playerLookComp->SetReticleActive( false );

            SendReticleActiveEvent( m_ui, id, false );
        } 

        // increment timer
        if (!playerLookComp->ReticleActive( ))
            playerLookComp->IncrementTimer( dt );

        // check if greater than delay
        if (playerLookComp->GetTimer( ) >= playerLookComp->GetDelayTime( ))
        {
            playerLookComp->SetTimer( 0.0f );

            auto currEnemy = playerLookComp->GetCurrentEnemy( );

            if (currEnemy == nullptr)
                continue;

            SendHealthTrackEndEvent( m_ui, id );

            // disconnect from death event
            // connect to enemyhit death event
            auto currEnemyHealth = playerLookComp->GetCurrentEnemyHealth( );
            if (currEnemyHealth != nullptr)
            {
                currEnemyHealth->Listener( playerLookComp )
                    .Off( HEALTH_ZERO, &PlayerLookAt::onEnemyDeath );
            }

            playerLookComp->SetCurrentEnemyHealth( nullptr );
            playerLookComp->SetCurrentEnemy( EntityHandle( nullptr ) );
        }
    }
}


