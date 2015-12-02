#include "Precompiled.h"

#include "HealthComponent.h"

NATIVE_COMPONENT_DEFINITION( Health );

Health::Health( )
    : BaseComponent( )
    , m_health( 100 )
{
    
}

int Health::GetHealth() const
{
    return m_health;
}

void Health::SetHealth( const int health )
{
    m_health = health;
}

void Health::DealDamage( const int damage )
{
    m_health -= damage;

    if(m_health <= 0)
    {
        GetOwner( )->Delete( );
    }
}