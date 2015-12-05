/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SystemConfig.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "UrsineTypes.h"

// Required at the top of all entity system declarations
#define ENTITY_SYSTEM                              \
    public:                                        \
        Meta(Enable)                               \
        static ursine::ecs::SystemTypeID SystemID; \
    private:                                       \

// Required in the translation unit of all entity systems
#define ENTITY_SYSTEM_DEFINITION(type)             \
    ursine::ecs::SystemTypeID type::SystemID = -1; \

// Determines if the world has the given system type
#define HasEntitySystem(systemType) GetSystemManager( )->HasSystem<systemType>( )

// Gets the given entity system from the world
#define GetEntitySystem(systemType) GetSystemManager( )->GetSystem<systemType>( )

/** @brief Disables auto adding of an entity system
*/
struct DisableEntitySystemAutoAdd : ursine::meta::MetaProperty
{
    META_OBJECT;
};

namespace ursine
{
    namespace ecs
    {
        // Type for unique ids mapped to entity system types.
        typedef uint64 SystemTypeID;

        // Type for unique bit masks mapped to entity system types.
        typedef uint64 SystemTypeMask;

        // Maximum number of entity systems able to be stored (number of bits able
        // to be stored in SystemTypeMask).
        const uint8 KMaxEntitySystemCount = sizeof( SystemTypeMask ) * kBitsPerByte;
    }
}