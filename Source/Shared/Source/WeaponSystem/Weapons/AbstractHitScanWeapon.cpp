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


namespace
{
    // Helper to check if the archetype to shoot needs to have .uatype appended to it
    void CheckArchetypeToShoot(std::string& archetype)
    {
        if ( archetype.find(".uatype") == std::string::npos )
            archetype += ".uatype";

        if ( archetype.find("FX/") == std::string::npos )
            archetype = "FX/" + archetype;
    }

}; // unamed namespace


AbstractHitscanWeapon::AbstractHitscanWeapon(void) :
    AbstractWeapon( ),
    m_raycastType( physics::RaycastType::RAYCAST_CLOSEST_HIT),
    m_drawDuration( 0.2f ),
    m_shotParticle( "FX/FX_bullet.uatype" ),
    m_trailParticle( "FX/FX_Trail.uatype" ),
    m_alwaysDraw( false ),
    m_debug( false )
{
}

ursine::physics::RaycastType AbstractHitscanWeapon::GetRaycastType( ) const
{
    return m_raycastType;
}

void AbstractHitscanWeapon::SetRaycastType(const ursine::physics::RaycastType type)
{
    m_raycastType = type;
}

float AbstractHitscanWeapon::GetDrawDuration( ) const
{
    return m_drawDuration;
}

void AbstractHitscanWeapon::SetDrawDuration(const float drawDuration)
{
    m_drawDuration = drawDuration;
}

const std::string& AbstractHitscanWeapon::GetShotParticle( ) const
{
    return m_shotParticle;
}

void AbstractHitscanWeapon::SetShotParticle(const std::string& particleArchetype)
{
    m_shotParticle = particleArchetype;

    CheckArchetypeToShoot(m_shotParticle);
}

const std::string& AbstractHitscanWeapon::GetTrailParticle( ) const
{
    return m_trailParticle;
}

void AbstractHitscanWeapon::SetTrailParticle(const std::string& particleArchetype)
{
    m_trailParticle = particleArchetype;

    CheckArchetypeToShoot(m_trailParticle);
}

bool AbstractHitscanWeapon::GetAlwaysDraw( ) const
{
    return m_alwaysDraw;
}

void AbstractHitscanWeapon::SetAlwaysDraw(const bool always)
{
    m_alwaysDraw = always;
}

bool AbstractHitscanWeapon::GetDebug( ) const
{
    return m_debug;
}

void AbstractHitscanWeapon::SetDebug(const bool debug)
{
    m_debug = debug;
}



