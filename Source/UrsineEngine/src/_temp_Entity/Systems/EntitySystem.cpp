/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EntitySystem.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/


#include "UrsinePrecompiled.h"

#include "EntitySystem.h"
#include "World.h"
#include "Entity.h"

namespace ursine
{
    namespace ecs
    {
        EntitySystem::EntitySystem(World *world, SystemPriority priority) 
            : _priority(priority)
            , _world(world)
            , _timers(kTimerGroupGameplay)
            , _tweens(kTweenGroupGameplay)
        { }

        bool EntitySystem::compare(const EntitySystem *a, const EntitySystem *b)
        {
            return a && b && b->_priority < a->_priority;
        }
    }
}