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

#include "Meta.h"

#include "ComponentConfig.h"
#include "EventDispatcher.h"

namespace ursine
{
    namespace ecs
    {
        class Entity;
		class Transform;

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
            // anything that derives can overload this to init their values
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

			// Utility class for storing a reference to an entity's component.
			// This is needed due to component pointers being invalidated, but entity
			// pointers being reliable
			template<class ComponentType>
			class Handle
			{
			public:
				Handle(void);
				Handle(const Handle<ComponentType> &);
				Handle(const ComponentType *);
				~Handle(void);

				// Assignment and Equivalency
				const ComponentType *operator=(const ComponentType *rhs);
				const Handle<ComponentType> &operator=(const Handle<ComponentType> &rhs);
				bool operator==(const ComponentType *rhs) const;
				bool operator==(const Handle<ComponentType> &rhs) const;
                bool operator!=(const ComponentType *rhs) const;
                bool operator!=(const Handle<ComponentType> &rhs) const;

				// Checking for null
				explicit operator bool(void) const;

				// Dereference Operators
				ComponentType &operator*(void);
				const ComponentType &operator*(void) const;
				ComponentType *operator->(void);
				const ComponentType *operator->(void) const;

			private:
				Entity *m_entity;
			};

        private:
        #if defined(URSINE_WITH_EDITOR)

            // whether or not OnInitialize has been called on the base component type
            bool m_baseInitialized;

        #endif

            // Called when the component has been initialized and added to an entity
            void Initialize(void);

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