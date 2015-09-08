/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ComponentConfig.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "UrsineTypes.h"
#include "Utils.h"

#include <vector>
#include <unordered_map>

// Gets the unique id of this component type
#define GetComponentID(componentType) componentType::ID

// Gets the unique mask for this component type
#define GetComponentMask(componentType) componentType::Mask

// Cleaner way of calling a base component constructor in the member 
// initialization list (used to assign the component type id)
#define BaseComponent() ursine::ecs::Component( GetComponentID( std::remove_reference< decltype( *this ) >::type ) )

namespace ursine
{
    class JsonSerializer;

    namespace ecs
    {
        class Component;

        // Type for unique ids mapped to component types
        typedef uint8 ComponentTypeID;

        // Type for IDs representing unique instances of components
        typedef uint32 ComponentUniqueID;

        // Type for unique bit masks mapped to component types
        typedef uint64 ComponentTypeMask;

        // Vector of component pointers
        typedef std::vector<Component*> ComponentVector;

        // Maximum number of components able to be stored (number of bits able
        // to be stored in ComponentTypeMask).
        const uint8 kMaxComponentCount = sizeof(ComponentTypeMask) * kBitsPerByte;
    }
}
