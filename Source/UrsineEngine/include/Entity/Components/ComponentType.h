/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ComponentType.h
** - Stores component type information
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "ComponentConfig.h"
#include "JsonSerializer.h"

namespace ursine
{
    namespace ecs
    {
        template<class Type>
        class ComponentType final
        {
        public:
            // ID of this component type
            static ComponentTypeID ID;

            // Bit mask for this component type
            static ComponentTypeMask Mask;

            // Determines if this type has been registered with the component registrar
            static bool Registered;

            // Registers a component
            ComponentType(const char *name);

            // Registers a component member
            ComponentType(const char *member_name,
                ComponentDataAccessor accessor,
                JsonSerializer::Serializer serializer,
                JsonSerializer::Deserializer deserializer,
                const char *display_name = nullptr);

            static Component *Create(void);
            static Component *Copy(const Component *instance);
        };
    }
}

#include "ComponentType.hpp"