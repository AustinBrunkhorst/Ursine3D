/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerLookAtComponent.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "PlayerLookAtComponent.h"
#include "ComponentIncludes.h"
#include "EntityEvent.h"
#include "PlayerIdComponent.h"
#include "UIEvents.h"
#include <UI/UIScreensConfigComponent.h>


NATIVE_COMPONENT_DEFINITION(PlayerLookAt);

using namespace ursine;


PlayerLookAt::PlayerLookAt(void) :
    BaseComponent( ),
    m_timer(0.0f),
    m_delay(2.0f),
    m_currEnemy(nullptr)
{
}

PlayerLookAt::~PlayerLookAt(void)
{
    if ( m_currEnemy != nullptr )
    {
        m_currEnemy->Listener(this)
            .Off(ursine::ecs::ENTITY_REMOVED, &PlayerLookAt::onEnemyDeath);
    }
}

float PlayerLookAt::GetDelayTime(void) const
{
    return m_delay;
}

void PlayerLookAt::SetDelayTime(float time)
{
    m_delay = time;
}

float PlayerLookAt::GetTimer(void) const
{
    return m_timer;
}

void PlayerLookAt::SetTimer(float time)
{
    m_timer = time;
}

void PlayerLookAt::IncrementTimer(float dt)
{
    m_timer += dt;
}

bool PlayerLookAt::ReticleActive(void) const
{
    return m_active;
}

void PlayerLookAt::SetReticleActive(bool active)
{
    m_active = active;
}

const ursine::ecs::EntityHandle& PlayerLookAt::GetCurrentEnemy(void) const
{
    return m_currEnemy;
}

void PlayerLookAt::SetCurrentEnemy(ursine::ecs::EntityHandle& entity)
{
    m_currEnemy = entity;
}

void PlayerLookAt::onEnemyDeath(EVENT_HANDLER(ursine::ecs::Entity))
{
    // dont have to check if entity has player id because
    // this would not be called if it did not have it

    ui_event::HealthTrackEnd trackEvent;
    trackEvent.enemyKilled = true;
    trackEvent.playerID = GetOwner( )->GetComponent< PlayerID >( )->GetID( );

    auto *ui = GetOwner( )->GetWorld( )->GetSettings( )->GetComponent<UIScreensConfig>( );

    if ( ui )
        ui->TriggerPlayerHUDEvent(trackEvent);

    SetTimer( 0.0f );

    m_currEnemy->Listener( this )
        .Off( ursine::ecs::ENTITY_REMOVED, &PlayerLookAt::onEnemyDeath );

    m_currEnemy = nullptr;
}

void PlayerLookAt::OnInitialize(void)
{
    //GetOwner( )->Listener( this )
    //    .On( game::FIRE_END, &BaseWeapon::TriggerReleased );
}


