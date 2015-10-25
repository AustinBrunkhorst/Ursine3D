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

#include "ComponentProperties.h"

#include <vector>

// Gets the unique id of this component type
#define GetComponentID(componentType) (componentType::ComponentID)

// Gets the unique mask for this component type
#define GetComponentMask(componentType) (1ull << componentType::ComponentID)

// Determines if a component of this type has been registered
#define IsComponentRegistered(componentType) (typeof( componentType ).IsValid( ))

// Cleaner way of calling a base component constructor in the member 
// initialization list (used to assign the component type id)
#define BaseComponent() ursine::ecs::Component( GetComponentID( std::remove_reference<decltype( *this )>::type ) )

// Required at the top of all native component declarations
#define NATIVE_COMPONENT                                 \
    META_OBJECT                                          \
    public:                                              \
        Meta(Enable)                                     \
        static ursine::ecs::ComponentTypeID ComponentID; \
    private:                                             \

// Required in the translation unit of all native components
#define NATIVE_COMPONENT_DEFINITION(type)                \
    ursine::ecs::ComponentTypeID type::ComponentID = -1; \

#if defined(URSINE_WITH_EDITOR)

#define NOTIFY_COMPONENT_CHANGED(displayName, value)                                           \
    auto *__owner = GetOwner( );                                                               \
    ursine::ecs::EditorComponentChangedArgs __e {                                              \
        ursine::ecs::WORLD_ENTITY_EDITOR_COMPONENT_CHANGED, __owner, this, displayName, value  \
    };                                                                                         \
    __owner->GetWorld( )->Dispatch( ursine::ecs::WORLD_ENTITY_EDITOR_COMPONENT_CHANGED, &__e ) \

#else

#define NOTIFY_COMPONENT_CHANGED(displayName, value)

#endif

namespace ursine
{
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
        const uint8 kMaxComponentCount = sizeof( ComponentTypeMask ) * kBitsPerByte;
    }
}
