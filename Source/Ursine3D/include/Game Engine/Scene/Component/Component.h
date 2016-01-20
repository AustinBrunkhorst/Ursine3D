/* ---------------------------------------------------------------------------
** Team Bear King
** � 2015 DigiPen Institute of Technology, All Rights Reserved.
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

#include "Meta.h"

#include "ComponentConfig.h"
#include "EventDispatcher.h"

namespace ursine
{
    namespace ecs
    {
        class Entity;

        class Component : public meta::Object
        {
        public:
            // can set the owner
            friend class EntityManager;
            friend class EntitySerializer;

            inline explicit Component(ComponentTypeID typeID);

            Component(const Component &rhs) = default;

            Component &operator=(const Component &rhs) = default;

            // Called when the component has been initialized and added to an entity
            virtual void OnInitialize(void);

            // The type ID of this component
            inline ComponentTypeID GetTypeID(void) const;

            // The type mask of this component
            inline ComponentTypeMask GetTypeMask(void) const;

            // Gets the owner of this component. Use it wisely
            inline Entity *GetOwner(void) const;

            // Determines if this component is of the specified type
            template<class ComponentType>
            inline bool Is(void) const;

        private:
        #if defined(URSINE_WITH_EDITOR)

            // whether or not OnInitialize has been called on the base component type
            bool m_baseInitialized;

        #endif

            // component type id
            ComponentTypeID m_typeID;

            // unique instance id
            ComponentUniqueID m_uniqueID;

            // entity that this component is attached to
            Entity *m_owner;
        } Meta(WhiteListMethods);
    }
}

#include "Component.hpp"