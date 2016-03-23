/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ProjectileComponent.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "ProjectileComponent.h"
#include "GameEvents.h"

NATIVE_COMPONENT_DEFINITION( Projectile );


Projectile::Projectile( void )
    : BaseComponent( )
    , m_speed( 10 )
    , m_lifeTime( 0 )
{
}

Projectile::~Projectile( void )
{
}

void Projectile::OnInitialize(void)
{
    GetOwner( )->Listener( this )
        .On( game::PROJECTILE_INIT, &Projectile::OnInit );
}

// projectile speed
float Projectile::GetSpeed( void ) const
{
    return m_speed;
}

void Projectile::SetSpeed( const float speed )
{
    m_speed = speed;
}

void Projectile::Update( const float dt )
{
    m_lifeTime -= dt;

    if ( m_lifeTime <= 0.0f )
    {
        GetOwner( )->Delete( );
    }
}

void Projectile::OnInit(EVENT_HANDLER(ursine::ecs::Entity))
{
    EVENT_ATTRS(ursine::ecs::Entity, game::ProjectileInitEventArgs);

    CalculateLifeTime(args->m_range);

    GetOwner( )->GetComponent<ursine::ecs::Rigidbody>( )
        ->SetVelocity(args->m_forwardVec * m_speed);
}

// calculate lifetime of projectile
void Projectile::CalculateLifeTime(float range)
{
    // infinite range
    if ( range == 0.0f )
    {
        range = FLT_MAX;
    }

    m_lifeTime = range / m_speed;
}


