/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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

#include "Array.h"
#include "ResourceManager.h"
#include "ResourceReference.h"

namespace ursine
{
    class Scene;

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

            // Called when all values have been initialized for this 
            // component from serialization
            virtual void OnInitialize(void) { }

            // Called when a new scene is prepared
            virtual void OnSceneReady(Scene *scene) { }

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

                    ComponentType *Get(void);
                    const ComponentType *Get(void) const;
                    
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

                Entity *GetEntity(void) const;

               private:
                    Entity *m_entity;
               };

        protected:
            // determines if resources are available
            bool resourcesAreAvailable(void) const;

            template<typename ResourceType>
            ResourceType *loadResource(const resources::ResourceReference &resource) const;

        private:
            // component type id
            ComponentTypeID m_typeID;

            ComponentTypeMask m_typeMask;

            // unique instance id
            ComponentUniqueID m_uniqueID;

            // entity that this component is attached to
            Entity *m_owner;

            void onInitialize(void);
            void onSceneReady(Scene *scene);
            
        } Meta(WhiteListMethods);
    }
}

#include "Component.hpp"