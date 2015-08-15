/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Component.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "ComponentType.h"
#include "ComponentRegistrar.h"

namespace Ursine
{
    namespace ECS
    {
        class Entity;

        class Component
        {
            // can set the owner
            friend class EntityManager;

            // component type id
            ComponentTypeID _type_id;

            // unique instance id
            ComponentUniqueID _unique_id;
            
            // Entity that this component is attached to.
            Entity *_owner;

            // component type mask (1 << _type_id)
            ComponentTypeMask _type_mask;
        public:
            explicit inline Component(ComponentTypeID type_id);

            Component(const Component &rhs) = default;
            Component &operator=(const Component &rhs) = default;

            virtual ~Component(void) { }

            // Called when the component has been initialized and added to an entity
            virtual void OnInitialize(void) { }

            // The type ID of this component
            inline ComponentTypeID GetTypeID(void) const;

            // The type mask of this component
            inline ComponentTypeMask GetTypeMask(void) const;

            // Gets the owner of this component. Use it wisely
            inline Entity *GetOwner(void) const;

            // The string name of this component type
            inline const std::string &GetName(void) const;

            // Determines if this component is of the specified type
            template<class ComponentType>
            inline bool Is(void) const;
        };
    }
}

#include "Component.hpp"