/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** HealthComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "HealthComponent.h"

#include <SystemManager.h>
#include <Systems/RoundSystem.h>
#include "TeamComponent.h"
#include "GameEvents.h"
#include "DamageOnCollideComponent.h"

NATIVE_COMPONENT_DEFINITION( Health );

Health::Health(void)
    : BaseComponent( )
    , m_health( 100 )
{
    
}

Health::~Health(void)
{
    
}

float Health::GetHealth(void) const
{
    return m_health;
}

void Health::SetHealth(const float health)
{
    m_health = health;
}

float Health::GetMaxHealth(void) const
{
    return m_maxHealth;
}

void Health::DealDamage(const float damage)
{
    auto owner = GetOwner( );

	if (owner->GetComponent<TeamComponent>( )->IsDead( ))
	{
		return;
	}

    m_health -= damage;

    auto *roundSystem = owner->GetWorld( )->GetEntitySystem( RoundSystem );

    if (m_health <= 0)
    {
		roundSystem->SendPlayerDiedMessage( owner );
    }
    else
    {
        roundSystem->SendPlayerDamageTaken( owner );
    }
}

void Health::OnInitialize(void)
{
    m_maxHealth = m_health;

    ConnectToAllCritSpots( );
}

void Health::ConnectToAllCritSpots(void)
{
    GetOwner( )->Listener(this).On( game::DAMAGE_EVENT, &Health::OnDamaged );

    ursine::ecs::Entity* entity;
    auto world = GetOwner( )->GetWorld( );
    auto children = GetOwner( )->GetChildren( );


    for ( auto it : *children )
    {
        entity = world->GetEntity( it );
        entity->Listener(this).On(game::DAMAGE_EVENT, &Health::OnDamaged);
    }
}


void Health::OnDamaged(EVENT_HANDLER(game::DAMAGE_EVENT))
{
    EVENT_ATTRS(ursine::ecs::Entity, game::DamageEventArgs);

    DealDamage(args->m_damage);

    args->m_damageComp->AddEntityToIntervals( GetOwner( )->GetUniqueID( ) );
}


