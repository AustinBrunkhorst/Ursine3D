/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SystemTag.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "SystemConfig.h"

namespace Ursine
{
    namespace ECS
    {
        template<class Type>
        struct SystemType final
        {
            SystemType(const char *name, SystemFactory factory, bool auto_add = true);

            // ID of this system type
            static SystemTypeID ID;

            // Bit mask for this system type
            static SystemTypeMask Mask;

            // Determines if this type has been registered with the entity system registrar
            static bool Registered;

            // Factory method for constructing a system of this type
            static SystemFactory Factory;
        };
    }
}

#include "SystemType.hpp"