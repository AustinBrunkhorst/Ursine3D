/* ----------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** AmmoPickupComponent.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "AmmoPickupComponent.h"
#include <CollisionEventArgs.h>

NATIVE_COMPONENT_DEFINITION( AmmoPickup ) ;


AmmoPickup::AmmoPickup(void) :
    BaseComponent( ),
    m_count( 10 )
{
}

AmmoPickup::~AmmoPickup(void)
{
}

int AmmoPickup::GetAmmoCount( ) const
{
    return m_count;
}

void AmmoPickup::SetAmmoCount(const int count)
{
    m_count = count;
}

void AmmoPickup::OnInitialize(void)
{
}


