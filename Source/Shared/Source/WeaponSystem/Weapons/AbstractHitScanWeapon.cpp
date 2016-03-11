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
    , m_drawDuration( 0.2f )
    , m_alwaysDraw( false )
    , m_debug( false ) { }

ursine::physics::RaycastType AbstractHitscanWeapon::GetRaycastType(void) const
{
    return m_raycastType;
}

void AbstractHitscanWeapon::SetRaycastType(const ursine::physics::RaycastType type)
{
    m_raycastType = type;
}

float AbstractHitscanWeapon::GetDrawDuration(void) const
{
    return m_drawDuration;
}

void AbstractHitscanWeapon::SetDrawDuration(const float drawDuration)
{
    m_drawDuration = drawDuration;
}

const ResourceReference &AbstractHitscanWeapon::GetShotParticle(void) const
{
    return m_shotParticle;
}

void AbstractHitscanWeapon::SetShotParticle(const ResourceReference &particleArchetype)
{
    m_shotParticle = particleArchetype;
}

const ResourceReference &AbstractHitscanWeapon::GetTrailParticle(void) const
{
    return m_trailParticle;
}

void AbstractHitscanWeapon::SetTrailParticle(const ResourceReference &particleArchetype)
{
    m_trailParticle = particleArchetype;
}

bool AbstractHitscanWeapon::GetAlwaysDraw(void) const
{
    return m_alwaysDraw;
}

void AbstractHitscanWeapon::SetAlwaysDraw(const bool always)
{
    m_alwaysDraw = always;
}

bool AbstractHitscanWeapon::GetDebug(void) const
{
    return m_debug;
}

void AbstractHitscanWeapon::SetDebug(const bool debug)
{
    m_debug = debug;
}
