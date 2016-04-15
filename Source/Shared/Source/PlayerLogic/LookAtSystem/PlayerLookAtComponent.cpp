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
#include "HealthComponent.h"
#include "UIEvents.h"
#include <UI/UIScreensConfigComponent.h>


NATIVE_COMPONENT_DEFINITION(PlayerLookAt);

using namespace ursine;


PlayerLookAt::PlayerLookAt(void) :
    BaseComponent( ),
    m_delay( 2.0f ),
    m_timer( 0.0f ),
    m_healthPercent( 0.0f ),
    m_currEnemy( nullptr ),
    m_currEnemyHealth( nullptr ),
    m_reticleActive( false )
{
}

PlayerLookAt::~PlayerLookAt(void)
{
    if ( m_currEnemyHealth != nullptr )
    {
        m_currEnemyHealth->Listener( this )
            .Off( HealthEvents::HEALTH_ZERO, &PlayerLookAt::onEnemyDeath );
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

float PlayerLookAt::GetHealthPercent(void) const
{
    return m_healthPercent;
}

void PlayerLookAt::SetHealthPercent(float percent)
{
    m_healthPercent = percent;
}

bool PlayerLookAt::ReticleActive(void) const
{
    return m_reticleActive;
}

void PlayerLookAt::SetReticleActive(bool active)
{
    m_reticleActive = active;
}

const ursine::ecs::EntityHandle& PlayerLookAt::GetCurrentEnemy(void) const
{
    return m_currEnemy;
}

void PlayerLookAt::SetCurrentEnemy(ursine::ecs::EntityHandle& entity)
{
    m_currEnemy = entity;
}

const ecs::Component::Handle<Health> &PlayerLookAt::GetCurrentEnemyHealth(void) const
{
    return m_currEnemyHealth;
}

void PlayerLookAt::SetCurrentEnemyHealth(const Handle<Health> &healthComp)
{
    m_currEnemyHealth = healthComp;
}

void PlayerLookAt::onEnemyDeath(EVENT_HANDLER(ursine::ecs::Entity))
{
    // dont have to check if entity has player id because
    // this would not be called if it did not have it

    auto *ui = GetOwner()->GetWorld()->GetSettings()->GetComponent<UIScreensConfig>();

    if (ui)
    {
        int id = GetOwner()->GetComponent< PlayerID >()->GetID();

        ui_event::HealthTrackEnd trackEvent;
        trackEvent.enemyKilled = true;
        trackEvent.playerID = id;

        ui->TriggerPlayerHUDEvent(trackEvent);

        // crete reticle event
        ui_event::ReticleActive reticleEvent;
        reticleEvent.playerID = id;
        reticleEvent.active = false;

        ui->TriggerPlayerHUDEvent(reticleEvent);
    }

    SetTimer(0.0f);

    if ( m_currEnemyHealth )
    {
        m_currEnemyHealth->Listener(this)
            .Off(HealthEvents::HEALTH_ZERO, &PlayerLookAt::onEnemyDeath);
    }

    m_currEnemy = nullptr;
    m_currEnemyHealth = nullptr;

    m_reticleActive = false;
}

void PlayerLookAt::OnInitialize(void)
{
    //GetOwner( )->Listener( this )
    //    .On( game::FIRE_END, &BaseWeapon::TriggerReleased );
}


