#pragma once

#include "Component.h"

namespace ursine
{
    namespace ecs
    {
        class Transform : public Component 
        {
            explicit Transform(ComponentTypeID typeID)
                : Component( typeID )
            {

            }

            explicit Transform(const Component& rhs)
                : Component( rhs )
            {

            }
        };
    }
}