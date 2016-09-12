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
        class EntityHandle;

        typedef uint32 EntityID;
        typedef uint32 EntityIDVersion;

        typedef std::vector<EntityHandle> EntityHandleVector;
        typedef std::vector<EntityID> EntityIDVector;
        typedef std::vector<std::string> EntityGroupVector;
    }
}