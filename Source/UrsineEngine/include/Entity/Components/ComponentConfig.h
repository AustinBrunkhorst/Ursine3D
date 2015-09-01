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

// Registers a component with the registrar.
// IMPORTANT: this must be done in the components header file.
#define RegisterComponent(component_type)                     \
    volatile static ursine::ecs::ComponentType<component_type>\
        register_##component_type(#component_type);           \

// Intellisense is a silly guy
#ifdef __INTELLISENSE__
// Registers component data with the registrar.
// IMPORTANT: this must be done in the components header file.
#define RegisterComponentData(component_type, member_name, ...)
#else
#define RegisterComponentData(component_type, member_name, ...)                                    \
    static void *access_##component_type##member_name(const ursine::ecs::Component *instance)      \
    {                                                                                              \
        return (void*)(&((const component_type*)instance)->member_name);                           \
    }                                                                                              \
    static ursine::Json serialize_##component_type##member_name(void *instance)                    \
    {                                                                                              \
        return ursine::JsonSerializer::Serialize(                                                  \
            *static_cast<decltype(component_type::member_name)*>(instance));                       \
    }                                                                                              \
    static void deserialize_##component_type##member_name(const ursine::Json &data, void *instance)\
    {                                                                                              \
        ursine::JsonSerializer::Deserialize(data,                                                  \
            *static_cast<decltype(component_type::member_name)*>(instance));                       \
    }                                                                                              \
    volatile static ursine::ecs::ComponentType<component_type>                                     \
        register_member_##component_type##member_name(                                             \
            #member_name,                                                                          \
            access_##component_type##member_name,                                                  \
            serialize_##component_type##member_name,                                               \
            deserialize_##component_type##member_name,                                             \
            ##__VA_ARGS__);                                                                        \

#endif

// Registers a dependency for this component type. After serialization, if 
// any component is missing a dependency, an error will be thrown.
#define RegisterComponentDependency(component_type, dependency)

// Allows the serializer to access private component data. Use this before
// any private fields in the class you wish to allow access.
#define SerializerAllow(component_type, member_name) friend void *access_##component_type##member_name(const ursine::ecs::Component * const instance)

// Gets the unique id of this component type
#define GetComponentID(component_type) ursine::ecs::ComponentRegistrar::GetID<component_type>()

// Gets the unique mask for this component type
#define GetComponentMask(component_type) ursine::ecs::ComponentRegistrar::GetMask<component_type>()

// Determines if a component of this type has been registered
#define IsComponentRegistered(component_type) ursine::ecs::ComponentRegistrar::IsRegistered<component_type>()

// Cleaner way of calling a base component constructor in the member 
// initialization list (used to assign the component type id)
#define BaseComponent() ursine::ecs::Component(GetComponentID(std::remove_reference<decltype(*this)>::type))

namespace ursine
{
    class JsonSerializer;

    namespace ecs
    {
        class Component;

        typedef void *(*ComponentDataAccessor)(const Component *);
        typedef Component *(*ComponentFactory)(void);

        typedef std::unordered_map<
            std::string, 
            std::pair<ComponentDataAccessor, JsonSerializer>
        > ComponentDataMap;

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
