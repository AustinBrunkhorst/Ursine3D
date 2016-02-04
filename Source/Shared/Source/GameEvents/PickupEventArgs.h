#pragma once

/* ---------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** PickupEventArgs.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <EntityEvent.h>
#include <string>

struct WeaponPickupEventArgs : ursine::EventArgs
{
    std::string weaponToPickup;

    WeaponPickupEventArgs(std::string& weapon)
        : weaponToPickup( weapon )
    { }
};

