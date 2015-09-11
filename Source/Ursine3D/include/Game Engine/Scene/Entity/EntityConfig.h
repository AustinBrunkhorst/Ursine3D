/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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

        // key declaring base entity type (in JSON)
        const std::string kEntityKeyBase = ".base";

        // key declaring component type in entity files, or entity type 
        // in world files
        const std::string kEntityKeyType = ".type";

        // key declaring the tag name for this entity
        const std::string kEntityKeyTag = ".tag";

        // key mapping the components for this entity (in JSON)
        const std::string kEntityKeyComponents = ".components";

        // extension for entity files
        const std::string kEntityExtension = ".entity";
    }
}