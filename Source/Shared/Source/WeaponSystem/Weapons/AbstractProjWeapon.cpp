
#include <Precompiled.h>
#include "AbstractProjWeapon.h"


using namespace ursine;

namespace
{
    // Helper to check if the archetype to shoot needs to have .uatype appended to it
    void CheckArchetypeToShoot(std::string& archetype)
    {
        if (archetype.find( ".uatype" ) == std::string::npos)
            archetype += ".uatype";
    }

}; // unamed namespace


AbstractProjWeapon::AbstractProjWeapon(void) :
    AbstractWeapon( ),
    m_projSpeed( 10.0f ),
    m_archetypeToShoot( "BaseBullet.uatype" )
{
}

// projectile speed  get/set
float AbstractProjWeapon::GetProjSpeed(void) const
{
    return m_projSpeed;
}

void AbstractProjWeapon::SetProjSpeed(const float speed)
{
    m_projSpeed = speed;
}


// Archetype To Shoot
const std::string& AbstractProjWeapon::GetArchetypeToShoot(void) const
{
    return m_archetypeToShoot;
}

void AbstractProjWeapon::SetArchetypeToShoot(const std::string& archetype)
{
    m_archetypeToShoot = archetype;

    CheckArchetypeToShoot( m_archetypeToShoot );
}


