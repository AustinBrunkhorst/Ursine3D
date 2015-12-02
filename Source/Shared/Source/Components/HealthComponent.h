#pragma once

#include <Component.h>

class Health : public ursine::ecs::Component
{
    NATIVE_COMPONENT;
public:
    Health( );

    int GetHealth( ) const;
    void SetHealth( const int health );

    void DealDamage( const int damage );

private:
    int m_health;

};
