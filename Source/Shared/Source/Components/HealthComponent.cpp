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
    Component::OnInitialize( );

    m_maxHealth = m_health;
}