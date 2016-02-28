#include "Precompiled.h"
#include "BaseWeaponComponent.h"
#include "GameEvents.h"

NATIVE_COMPONENT_DEFINITION( BaseWeapon );

using namespace ursine;

namespace
{
    // Helper to check if the archetype to shoot needs to have .uatype appended to it
    void CheckArchetypeToShoot(std::string& archetype)
    {
        if ( archetype.find(".uatype") == std::string::npos )
            archetype += ".uatype";
    }

}; // unamed namespace



BaseWeapon::BaseWeapon( void ) :
    BaseComponent( ),
    AbstractWeapon( ),
    m_archetypeToShoot("BaseBullet.uatype")
{
}

BaseWeapon::~BaseWeapon( void )
{
    AbstractWeaponDisconnect( BaseWeapon );
}

void BaseWeapon::OnInitialize( void )
{
    AbstractWeaponInit( BaseWeapon, GetOwner( ) );
}

// Archetype To Shoot
const std::string& BaseWeapon::GetArchetypeToShoot(void) const
{
    return m_archetypeToShoot;
}

void BaseWeapon::SetArchetypeToShoot(const std::string& archetype)
{
    m_archetypeToShoot = archetype;

    CheckArchetypeToShoot(m_archetypeToShoot);
}

void BaseWeapon::RemoveMySelf(void)
{
    GetOwner( )->Delete( );
}

