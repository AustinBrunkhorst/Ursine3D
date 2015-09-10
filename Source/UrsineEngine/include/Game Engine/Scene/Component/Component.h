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

namespace ursine
{
    namespace ecs
    {
        class Entity;

        class Component
        {
            // can set the owner
            friend class EntityManager;

            // component type id
            ComponentTypeID m_typeID;

            // unique instance id
            ComponentUniqueID m_uniqueID;
            
            // Entity that this component is attached to.
            Entity *m_owner;

            // component type mask (1 << m_typeID)
            ComponentTypeMask m_typeMask;
        public:
            explicit inline Component(ComponentTypeID typeID);

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

            // Determines if this component is of the specified type
            template<class ComponentType>
            inline bool Is(void) const;
        };
    }
}

#include "Component.hpp"