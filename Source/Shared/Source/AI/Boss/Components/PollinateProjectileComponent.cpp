/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** PollinateProjectileComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "PollinateProjectileComponent.h"

#include <GhostComponent.h>
#include <Application.h>

NATIVE_COMPONENT_DEFINITION(PollinateProjectile);

using namespace ursine;
using namespace ecs;

PollinateProjectile::PollinateProjectile(void)
    : BaseComponent( )
    , m_direction( 1.0f, 0.0f, 0.0f )
    , m_gravity( 10.0f )
    , m_spreadDistance( 30.0f )
    , m_spreadTime( 2.0f )
    , m_lifeTime( 15.0f )
    , m_spreadTimer( 0.0f )
    , m_lifeTimer( 0.0f )
    , m_dissipateTime( 3.0f )
    , m_dissipateTimer( 0.0f ) { }

void PollinateProjectile::OnSceneReady(Scene *scene)
{
    GetOwner( )->GetWorld( )->Listener( this )
        .On( WORLD_UPDATE, &PollinateProjectile::onUpdate );
}

void PollinateProjectile::InitializeComponents(void)
{
    // Setup the rigidbody and the damageoncollide components
    auto rigidbody = GetOwner( )->GetComponent<Rigidbody>( );
    auto ghost = GetOwner( )->GetComponentInChildren<Ghost>( );
    auto damage = GetOwner( )->GetComponent<DamageOnCollide>( );
    auto emitter = GetOwner( )->GetComponent<ParticleEmitter>( );

    UAssert( rigidbody && ghost && damage && emitter, "Error: The archetype isn't setup properly." );

    rigidbody->SetBodyFlag( BodyFlag::Dynamic );
    rigidbody->SetMass( 1.0f );
    rigidbody->SetGravity( SVec3::UnitY( ) * -m_gravity );
    rigidbody->SetDisableContactResponse( false );
    rigidbody->SetEnableContactCallback( false );

    ghost->SetDisableContactResponse( true );
    ghost->SetEnableContactCallback( true );

    damage->SetListenToChildren( true );
    damage->SetDeleteOnCollision( false );
    damage->SetDamageType( DAMAGE_PLAYER );

    m_dissipateTime = emitter->GetLifetime( );
}

void PollinateProjectile::SetDirection(const SVec3 &direction)
{
    m_direction = direction;
}

void PollinateProjectile::SetGravity(float gravity)
{
    m_gravity = gravity;
}

void PollinateProjectile::SetSpreadDistance(float spreadDistance)
{
    m_spreadDistance = spreadDistance;
}

void PollinateProjectile::SetSpreadTime(float spreadTime)
{
    m_spreadTime = spreadTime;
}

void PollinateProjectile::SetLifeTime(float lifeTime)
{
    m_lifeTime = lifeTime;
}

void PollinateProjectile::onUpdate(EVENT_HANDLER(World))
{
    auto dt = Application::Instance->GetDeltaTime( );
    auto trans = GetOwner( )->GetTransform( );

    // Move it in the spread direction
    if (m_spreadTimer < m_spreadTime)
    {
        float t = m_spreadTimer / m_spreadTime;
        float speed = m_spreadDistance / m_spreadTime;

        GetOwner( )->GetComponent<Rigidbody>( )->SetVelocity(
            t * speed * m_direction + m_gravity * -SVec3::UnitY( )
        );

        m_spreadTimer += dt;
    }

    if (m_lifeTimer < m_lifeTime)
    {
        m_lifeTimer += dt;

        if (m_lifeTimer >= m_lifeTime)
        {
            // Turn off the emitter
            GetOwner( )->GetComponent<ParticleEmitter>( )->SetEmitRate( 0.0f );

            // Turn off the ghost collider and set damage to zero
            GetOwner( )->GetComponentInChildren<Ghost>( )->SetEnableContactCallback( false );
            GetOwner( )->GetComponent<DamageOnCollide>( )->SetDamageToApply( 0.0f );
        }
    }
    else if (m_dissipateTimer < m_dissipateTime)
    {
        m_dissipateTimer += dt;
    }
    else
    {
        GetOwner( )->GetWorld( )->Listener( this )
            .Off( WORLD_UPDATE, &PollinateProjectile::onUpdate );

        GetOwner( )->Delete( );
    }
}
