/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** HealthRegenComponent.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "HealthRegenComponent.h"
#include "ComponentIncludes.h"
#include "HealthComponent.h"

NATIVE_COMPONENT_DEFINITION( HealthRegen ) ;

using namespace ursine;


HealthRegen::HealthRegen(void) :
    BaseComponent( ),
    m_healthRegenTime( 6.0f ),
    m_shieldRegenTime( 3.0f ),
    m_waitTime( 2.0f ),
    m_timer( m_waitTime ),
    m_healthToAdd( 0.0f )
{
}

HealthRegen::~HealthRegen(void)
{
    auto health = GetOwner( )->GetComponent<Health>( );
    
    if (health)
        health->Listener(this)
            .Off( HealthEvents::HEALTH_DAMAGED, &HealthRegen::OnDamaged );
}

float HealthRegen::GetWaitTime(void) const
{
    return m_waitTime;
}

void  HealthRegen::SetWaitTime(float time)
{
    m_waitTime = time;
    m_timer = m_waitTime;
}

float HealthRegen::GetTimer(void) const
{
    return m_timer;
}

void HealthRegen::IncrementTimer(float dt)
{
    m_timer += dt;
}

void HealthRegen::ResetTimer(void)
{
    m_timer = 0.0f;
}

float HealthRegen::GetHealthToAdd(void) const
{
    return m_healthToAdd;
}

void HealthRegen::SetHealthToAdd(float healthToAdd)
{
    m_healthToAdd = healthToAdd;
}

float HealthRegen::GetShieldToAdd(void) const
{
    return m_shieldToAdd;
}

void HealthRegen::SetShieldToAdd(float shieldToAdd)
{
    m_shieldToAdd = shieldToAdd;
}

void HealthRegen::OnInitialize(void)
{
    GetOwner( )->GetComponent<Health>( )->Listener( this )
        .On( HealthEvents::HEALTH_DAMAGED, &HealthRegen::OnDamaged );
}

void HealthRegen::OnDamaged(EVENT_HANDLER(ursine::ecs::Entity))
{
    m_timer = 0.0f;
}
