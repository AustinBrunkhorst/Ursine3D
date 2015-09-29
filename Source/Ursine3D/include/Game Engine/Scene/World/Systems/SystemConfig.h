/* ---------------------------------------------------------------------------
** Team Bear King
** � 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SystemConfig.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "UrsineTypes.h"

namespace ursine
{
    namespace ecs
    {
        // forward declarations
        class EntitySystem;
        class World;

        // Type for unique ids mapped to entity system types.
        typedef uint64 SystemTypeID;

        // Type for unique bit masks mapped to entity system types.
        typedef uint64 SystemTypeMask;

        // Update priority of systems
        typedef int SystemPriority;

        // Maximum number of entity systems able to be stored (number of bits able
        // to be stored in EntitySystemTypeMask).
        const uint8 MAX_ENTITY_SYSTEM_COUNT = sizeof(SystemTypeMask) * kBitsPerByte;
    }
}