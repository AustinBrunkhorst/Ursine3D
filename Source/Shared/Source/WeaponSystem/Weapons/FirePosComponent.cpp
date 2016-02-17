/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** FirePos.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "FirePosComponent.h"
#include "ComponentIncludes.h"

NATIVE_COMPONENT_DEFINITION( FirePos ) ;

using namespace ursine;


FirePos::FirePos(void) :
    BaseComponent( )
{
}

FirePos::~FirePos(void)
{
    //GetOwner( )->Listener( this )
    //     .Off( game::FIRE_END, &BaseWeapon::TriggerReleased ); 
}


