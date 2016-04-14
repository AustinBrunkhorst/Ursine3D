
/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
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
#include <UI/UIScreensConfigComponent.h>

ENTITY_SYSTEM_DEFINITION( PlayerLookAtSystem ) ;

using namespace ursine;
using namespace ursine::ecs;

namespace
{
} // unnamed namespace


//////////////////////////////
////  Base Weapon System  ////
//////////////////////////////

PlayerLookAtSystem::PlayerLookAtSystem(ursine::ecs::World* world)
    : FilterSystem( world, Filter( ).All< PlayerLookAt, RaycastComponent, PlayerID >( ) )
{
}


void PlayerLookAtSystem::Enable(const ursine::ecs::EntityHandle& entity)
{
    m_playerLookAtComps[ entity ] = entity->GetComponent< PlayerLookAt >( );

    m_playerIDs[ entity ] = entity->GetComponent< PlayerID >( )->GetID( );

    m_raycastComps[ entity ] = entity->GetComponent< RaycastComponent >( );
}

void PlayerLookAtSystem::Disable(const ursine::ecs::EntityHandle& entity)
{
    m_playerLookAtComps.erase( entity );
    m_playerIDs.erase( entity );
    m_raycastComps.erase( entity );
}

void PlayerLookAtSystem::onUpdate(EVENT_HANDLER(World))
{
    float dt = Application::Instance->GetDeltaTime( );

    RaycastComponent* rayComp;
    PlayerLookAt* playerLookComp;
    Health* enemyHealthComp;

    EntityHandle enemyHandle;

    // send ui Event
    auto *ui = m_world->GetSettings( )->GetComponent< UIScreensConfig >( );
    
    int id;

    for ( auto playerLookIt : m_playerLookAtComps )
    {
        rayComp = m_raycastComps[ playerLookIt.first ];

        playerLookComp = playerLookIt.second;

        id = m_playerIDs[ playerLookIt.first ];

        enemyHandle = rayComp->GetEntityHit( );

        if ( enemyHandle != nullptr )
        {
            enemyHealthComp = enemyHandle->GetComponent< Health >( );

            // if not on entity then on root
            if ( enemyHealthComp == nullptr )
                enemyHealthComp = enemyHandle->GetRoot( )->GetComponent< Health >( );

            // new obj
            if ( enemyHandle != playerLookComp->GetCurrentEnemy( ) )
            {
                // unsubsribe to old enemies death
                playerLookComp->GetCurrentEnemy( )->Listener(playerLookComp)
                    .Off(ursine::ecs::ENTITY_REMOVED, &PlayerLookAt::onEnemyDeath);

                // change curr enemy
                playerLookComp->SetCurrentEnemy( enemyHandle );
                playerLookComp->SetReticleActive( false );

                if ( ui )
                {
                    // create track event for ui
                    ui_event::HealthTrackStart trackEvent;

                    trackEvent.playerID = id;
                    trackEvent.enemyName = "Enemy";
                    trackEvent.healthPercent = enemyHealthComp->GetHealth( ) / enemyHealthComp->GetMaxHealth( );

                    ui->TriggerPlayerHUDEvent( trackEvent );

                    // crete reticle event
                    ui_event::ReticleActive reticleEvent;
                    reticleEvent.playerID = id;
                    reticleEvent.active = true;

                    ui->TriggerPlayerHUDEvent( reticleEvent );
                }

                // connect to enemyhit death event
                enemyHandle->Listener( playerLookComp )
                    .On( ursine::ecs::ENTITY_REMOVED, &PlayerLookAt::onEnemyDeath );
            }

            // not new
            else if ( ui )
            {
                // create track update event
                ui_event::HealthTrackUpdate trackEvent;

                trackEvent.playerID = id;
                trackEvent.healthPercent = enemyHealthComp->GetHealth( ) / enemyHealthComp->GetMaxHealth( );

                ui->TriggerPlayerHUDEvent( trackEvent );
            }

            // set timer to 0.0f
            playerLookComp->SetTimer( 0.0f );
        }
        
        // not looking at enemy
        else if ( playerLookComp->ReticleActive( ) )
        {
            playerLookComp->SetReticleActive( false );

            if ( ui )
            {
                // crete reticle event
                ui_event::ReticleActive reticleEvent;
                reticleEvent.playerID = id;
                reticleEvent.active = false;

                ui->TriggerPlayerHUDEvent( reticleEvent );
            }

        }

        // increment timer
        if (playerLookComp->GetCurrentEnemy( ))
            playerLookComp->IncrementTimer( dt );

        // check if greater than delay
        if ( playerLookComp->GetTimer( ) >= playerLookComp->GetDelayTime( ) )
        {
            playerLookComp->SetTimer( 0.0f );

            if ( playerLookComp->GetCurrentEnemy( ) == nullptr )
                continue;

            if ( ui )
            {
                // create track end event
                ui_event::HealthTrackEnd trackEvent;

                trackEvent.playerID = id;
                trackEvent.enemyKilled = false;

                ui->TriggerPlayerHUDEvent( trackEvent );
            }

            // disconnect from death event
            // connect to enemyhit death event
            playerLookComp->GetCurrentEnemy( )->Listener( playerLookComp )
                .Off( ursine::ecs::ENTITY_REMOVED, &PlayerLookAt::onEnemyDeath );
        }
    }
}
