/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** WallComponent.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Precompiled.h>
#include <WallComponent.h>
#include "ComponentIncludes.h"

NATIVE_COMPONENT_DEFINITION( WallComponent ) ;

using namespace ursine;


WallComponent::WallComponent(void) :
    BaseComponent( )
{
}

WallComponent::~WallComponent(void)
{
    //GetOwner( )->Listener( this )
    //     .Off( game::FIRE_END, &BaseWeapon::TriggerReleased ); 
}

void WallComponent::OnInitialize(void)
{
    //GetOwner( )->Listener( this )
    //    .On( game::FIRE_END, &BaseWeapon::TriggerReleased );
}




