#include "Precompiled.h"

#include "HealthComponent.h"

NATIVE_COMPONENT_DEFINITION( Health );

Health::Health(void)
    : BaseComponent( )
    , m_health( 100 )
{
    
}

Health::~Health(void)
{
    
}

int Health::GetHealth(void) const
{
    return m_health;
}

void Health::SetHealth(const int health)
{
    m_health = health;
}

void Health::DealDamage(const int damage)
{
    m_health -= damage;

    if(m_health <= 0)
    {
        GetOwner( )->Delete( );
    }
}