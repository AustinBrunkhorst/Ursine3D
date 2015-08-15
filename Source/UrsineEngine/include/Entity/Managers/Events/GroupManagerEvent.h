/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GroupManagerEvent.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Entity.h"

#include "EventArgs.h"

#include <string>

namespace Ursine
{
    namespace ECS
    {
        enum GroupManagerEvent
        {
            // Entity was added to a group
            GM_GROUP_ADDED,

            // Entity was removed from a group
            GM_GROUP_REMOVED
        };

        struct GroupEventArgs : public EventArgs
        {
            Entity *entity;
            std::string group;

            GroupEventArgs(Entity *entity, const std::string &group)
                : entity(entity)
                , group(group) {}
        };
    }
}