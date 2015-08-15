/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ComponentSerializer.h
** - Serialize and deserialize component
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Json.h"

namespace Ursine
{
    namespace ECS
    {
        class Component;

        class ComponentSerializer final
        {
        public:
            static void Serialize(Json &target, const Component *component);
            static void Deserialize(const Json &obj, Component *target);
        };
    }
}
