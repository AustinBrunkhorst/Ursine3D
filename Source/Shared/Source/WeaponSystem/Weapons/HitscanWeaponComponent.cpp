/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** HitscanWeaponComponent.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "HitscanWeaponComponent.h"
#include "ComponentIncludes.h"

NATIVE_COMPONENT_DEFINITION( HitscanWeapon ) ;

using namespace ursine;

HitscanWeapon::HitscanWeapon(void) :
    BaseComponent( ),
    AbstractHitscanWeapon( )
{
}

HitscanWeapon::~HitscanWeapon(void)
{
    AbstractWeaponDisconnect(HitscanWeapon);
}

void HitscanWeapon::OnInitialize(void)
{
    AbstractWeaponInit(HitscanWeapon, GetOwner( ));
}

void HitscanWeapon::RemoveMySelf(void)
{
    GetOwner( )->Delete( );
}
