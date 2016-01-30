/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EntityConfig.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "UrsineTypes.h"

#include <vector>

namespace ursine
{
    namespace ecs
    {
        class Entity;

        typedef uint32 EntityID;
        typedef uint32 EntityUniqueID;

        typedef std::vector<Entity*> EntityVector;
        typedef std::vector<std::string> EntityGroupVector;
    }
}