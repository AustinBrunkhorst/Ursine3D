/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TagSystem.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "TagSystem.h"

namespace Ursine
{
    namespace ECS
    {
        TagSystem::TagSystem(World *world, SystemPriority priority)
            : EntitySystem(world, priority)
        {
            
        }
    }
}
