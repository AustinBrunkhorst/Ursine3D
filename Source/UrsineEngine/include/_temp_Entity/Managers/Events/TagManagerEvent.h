/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TagManagerEvent.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Entity.h"

#include <string>

namespace ursine
{
    namespace ecs
    {
        enum TagManagerEvent
        {
            TM_REGISTERED,
            TM_UNREGISTERED
        };

        struct TagEventArgs : public EventArgs
        {
            std::string tag;
            Entity *entity;

            TagEventArgs(const std::string &string, Entity *entity)
                : tag(tag)
                , entity(entity) {}
        };
    }
}