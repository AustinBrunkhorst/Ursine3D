/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ComponentReflection.h
** - Reflect component data
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "ComponentConfig.h"

namespace ursine
{
    namespace ecs
    {
        struct ComponentReflection
        {
            std::string name;
            ComponentTypeMask mask;
            ComponentDataMap members;

            ComponentReflection(void) = default;
        };
    }
}
