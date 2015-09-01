/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TagSystem.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "EntitySystem.h"

namespace ursine
{
    namespace ecs
    {
        class TagSystem : public EntitySystem
        {
        public:
            TagSystem(World *world, SystemPriority priority = 0);
        };
    }
}
