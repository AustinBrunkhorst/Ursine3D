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

// Gets the unique id of this entity system type
#define GetEntitySystemID(system_type) Ursine::ECS::SystemRegistrar::GetID<system_type>()

// Registers a system with the registrar using magic
#define RegisterEntitySystem(system_type, ...)                                                                               \
    static Ursine::ECS::EntitySystem *_factory_##system_type(Ursine::ECS::World *world)                                      \
    {                                                                                                                        \
        return static_cast<Ursine::ECS::EntitySystem*>(new system_type(world));                                              \
    }                                                                                                                        \
    volatile static Ursine::ECS::SystemType<system_type> _##system_type(#system_type, _factory_##system_type,##__VA_ARGS__); \
                                                                                                                

// Gets the unique mask for this entity system type
#define GetEntitySystemMask(system_type) Ursine::ECS::SystemRegistrar::GetMask<system_type>()

// Determines if a system of this type has been registered
#define IsEntitySystemRegistered(system_type) Ursine::ECS::SystemRegistrar::IsRegistered<system_type>()

#define GetEntitySystem(system_type) Manager<SystemManager>()->System<system_type>()

namespace Ursine
{
    namespace ECS
    {
        // forward declarations
        class EntitySystem;
        class World;

        // Constructs an entity system
        typedef EntitySystem *(*SystemFactory)(World *world);

        // Type for unique ids mapped to entity system types.
        typedef uint64 SystemTypeID;

        // Type for unique bit masks mapped to entity system types.
        typedef uint64 SystemTypeMask;

        // Update priority of systems
        typedef int SystemPriority;

        // Maximum number of entity systems able to be stored (number of bits able
        // to be stored in EntitySystemTypeMask).
        const uint8 MAX_ENTITY_SYSTEM_COUNT = sizeof(SystemTypeMask) * kBitsPerByte;
    }
}