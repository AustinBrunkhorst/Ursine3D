/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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
#include <bitset>

// Gets the unique id of this component type
#define GetComponentID(componentType) (componentType::ComponentID)

// Gets the unique mask for this component type
#define GetComponentMask(componentType) (componentType::TypeMask)

// Determines if a component of this type has been registered
#define IsComponentRegistered(componentType) (typeof( componentType ).IsValid( ))

// Cleaner way of calling a base component constructor in the member 
// initialization list (used to assign the component type id).
#define BaseComponent() ursine::ecs::Component( ComponentID )

// Required at the top of all native component declarations
#define NATIVE_COMPONENT                                             \
    META_OBJECT                                                      \
    public:                                                          \
        Meta(Enable)                                                 \
        static ursine::ecs::ComponentTypeID ComponentID;             \
        static ursine::ecs::ComponentTypeMask TypeMask;              \
        Meta(Enable)                                                 \
        static void SetComponentID(ursine::ecs::ComponentTypeID id); \
    private:                                                         \

// Required in the translation unit of all native components
#define NATIVE_COMPONENT_DEFINITION(type)                                              \
    ursine::ecs::ComponentTypeID type::ComponentID = ursine::ecs::kInvalidComponentID; \
    ursine::ecs::ComponentTypeMask type::TypeMask;                                     \
    void type::SetComponentID(ursine::ecs::ComponentTypeID id)                         \
    {                                                                                  \
        ComponentID = id;                                                              \
        TypeMask.set( id, true );                                                      \
    }                                                                                  \

#if defined(URSINE_WITH_EDITOR)

#define NOTIFY_COMPONENT_CHANGED(displayName, value)                                                    \
    {                                                                                                   \
        auto &__owner = GetOwner( );                                                                    \
        if (__owner)                                                                                    \
        {                                                                                               \
            ursine::ecs::EditorComponentChangedArgs __e {                                               \
                ursine::ecs::WORLD_EDITOR_ENTITY_COMPONENT_CHANGED, __owner, this, displayName, value   \
            };                                                                                          \
            __owner->GetWorld( )->Dispatch( __e.type, &__e );                                           \
        }                                                                                               \
    }                                                                                                   

#else

#define NOTIFY_COMPONENT_CHANGED(displayName, value)

#endif

namespace ursine
{
    namespace ecs
    {
        class Component;

        // Maximum number of components able to be stored (number of bits able
        // to be stored in ComponentTypeMask).
        const uint32 kMaxComponentCount = 256;

        // Type for unique ids mapped to component types
        typedef uint32 ComponentTypeID;

        // Type for IDs representing unique instances of components
        typedef uint32 ComponentUniqueID;

        // Type for unique bit masks mapped to component types
        typedef std::bitset<kMaxComponentCount> ComponentTypeMask;

        // Vector of component pointers
        typedef std::vector<Component*> ComponentVector;

        // Default value for uninitialized components
        const ComponentTypeID kInvalidComponentID = -1;
    }
}
