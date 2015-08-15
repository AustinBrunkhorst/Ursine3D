/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** WorldConfig.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <string>

namespace Ursine
{
    namespace ECS
    {
        const std::string kWorldKeySystems = "systems";
        const std::string kWorldKeyAttributes = "attributes";
        const std::string kWorldKeyEntities = "entities";
        const std::string kWorldExtension = ".world";
    }
}