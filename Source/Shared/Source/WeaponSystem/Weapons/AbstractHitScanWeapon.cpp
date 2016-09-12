/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** HitScanWeapon.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Precompiled.h>

#include "AbstractHitScanWeapon.h"

using namespace ursine;
using namespace ecs;
using namespace resources;

AbstractHitscanWeapon::AbstractHitscanWeapon(void) 
    : AbstractWeapon( )
    , m_raycastType( physics::RaycastType::RAYCAST_CLOSEST_HIT )
    , m_damageType(DAMAGE_ENEMY)
    , m_drawDuration( 0.2f )
    , m_damageToApply(1.0f)
    , m_critModifier(1.0f)
    , m_realDamage( m_damageToApply )
    , m_alwaysDraw( false )
    , m_maxDamage( false )
    , m_debug( false ) { }

// Raycast type
ursine::physics::RaycastType AbstractHitscanWeapon::GetRaycastType(void) const
{
    return m_raycastType;
}

void AbstractHitscanWeapon::SetRaycastType(const ursine::physics::RaycastType type)
{
    m_raycastType = type;
}

// Draw Duration
float AbstractHitscanWeapon::GetDrawDuration(void) const
{
    return m_drawDuration;
}

void AbstractHitscanWeapon::SetDrawDuration(const float drawDuration)
{
    m_drawDuration = drawDuration;
}

// Particle to spawn on object hit
const ResourceReference &AbstractHitscanWeapon::GetShotParticle(void) const
{
    return m_shotParticle;
}

void AbstractHitscanWeapon::SetShotParticle(const ResourceReference &particleArchetype)
{
    m_shotParticle = particleArchetype;
}

// Particle trail
const ResourceReference &AbstractHitscanWeapon::GetTrailParticle(void) const
{
    return m_trailParticle;
}

void AbstractHitscanWeapon::SetTrailParticle(const ResourceReference &particleArchetype)
{
    m_trailParticle = particleArchetype;
}

// Draw Raycast
bool AbstractHitscanWeapon::GetAlwaysDraw(void) const
{
    return m_alwaysDraw;
}

void AbstractHitscanWeapon::SetAlwaysDraw(const bool always)
{
    m_alwaysDraw = always;
}

// Raycast Debug draw
bool AbstractHitscanWeapon::GetDebug(void) const
{
    return m_debug;
}

void AbstractHitscanWeapon::SetDebug(const bool debug)
{
    m_debug = debug;
}

//// Damage Type
DamageType AbstractHitscanWeapon::GetDamageType(void) const
{
    return m_damageType;
}

void AbstractHitscanWeapon::SetDamageType(DamageType type)
{
    m_damageType = type;
}

void AbstractHitscanWeapon::ToggleMaxDamage(void)
{
    if (m_maxDamage)
    {
        SetDamage( m_realDamage );
        m_maxDamage = false;
    }
    else
    {
        SetDamage( MAX_GUN_DAMAGE );
        m_maxDamage = true;
    }
}

// Damage to apply
float AbstractHitscanWeapon::GetDamageToApply(void) const
{
    return m_damageToApply;
}

void AbstractHitscanWeapon::SetDamageToApply(float damage)
{
    m_damageToApply = damage;
    m_realDamage = damage;
}

void AbstractHitscanWeapon::SetDamage(float damage)
{
    m_damageToApply = damage;
}

// Crit modifier
float AbstractHitscanWeapon::GetCritModifier(void) const
{
    return m_critModifier;
}

void AbstractHitscanWeapon::SetCritModifier(float modifier)
{
    m_critModifier = modifier;
}




