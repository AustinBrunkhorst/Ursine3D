/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AbstractComponent.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "AbstractProjWeapon.h"

using namespace ursine;

AbstractProjWeapon::AbstractProjWeapon(void) 
    : AbstractWeapon( )
{ }

// Archetype To Shoot
const resources::ResourceReference &AbstractProjWeapon::GetArchetypeToShoot(void) const
{
    return m_archetypeToShoot;
}

void AbstractProjWeapon::SetArchetypeToShoot(const resources::ResourceReference &archetype)
{
    m_archetypeToShoot = archetype;
}
