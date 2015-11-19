/* ---------------------------------------------------------------------------
** Team Bear King
** � 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EntityManager.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "EntityManager.h"
#include "Entity.h"
#include "Filter.h"
#include "TransformComponent.h"
#include <queue>

namespace ursine
{
    namespace ecs
    {
        namespace
        {
            void configureComponents(void)
            {
                static bool configured = false;

                if (configured)
                    return;

                configured = true;

                auto componentType = typeof( Component );

                ComponentTypeID nextID = 0;

                for (auto derived : componentType.GetDerivedClasses( ))
                {
                    auto componentID = derived.GetStaticField( "ComponentID" );

                    UAssert( componentID.IsValid( ),
                        "Native component '%s' doesn't have a static field ComponentID.\n"
                        "Most likely missing NATIVE_COMPONENT in declaration",
                        derived.GetName().c_str( )
                    );

                    componentID.SetValue( nextID++ );

                    auto defaultCtor = derived.GetDynamicConstructor( );

                    UAssert( defaultCtor.IsValid( ), 
                        "Component type '%s' doesn't have a default dynamic constructor.",
                        derived.GetName( ).c_str( )
                    )
                }
            }
        }

        ////////////////////////////////////////////////////////////////////////
        // Constructors/Destructors
        ////////////////////////////////////////////////////////////////////////

        EntityManager::EntityManager(World *world)
            : WorldManager( world )
            , m_nextEntityID( 0 )
            , m_nextEntityUID( 0 )
            , m_nextComponentUID( 0 )
        {
            configureComponents( );
        }

        EntityManager::~EntityManager(void)
        {
            while (m_active.size( ) > 0)
            {
                auto entity = m_active[ 0 ];
                Remove( entity );
            }
        }

        ////////////////////////////////////////////////////////////////////////
        // Public Methods
        ////////////////////////////////////////////////////////////////////////

        Entity *EntityManager::Create(void)
        {
            auto entity = create( );

            entity->m_transform = entity->AddComponent<Transform>( );

            dispatchCreated( entity );

            return entity;
        }

        const EntityVector &EntityManager::GetActiveEntities(void) const
        {
            return m_active;
        }

        void EntityManager::AddComponent(Entity *entity, Component *component)
        {
            UAssert( !entity->HasComponent( component->GetTypeMask( ) ),
                "Component already exists: %s",
                component->GetType( ).GetName( ).c_str( ) );

            addComponent( entity, component );

            component->OnInitialize( );

            ComponentEventArgs e( WORLD_ENTITY_COMPONENT_ADDED, entity, component );

            m_world->Dispatch( WORLD_ENTITY_COMPONENT_ADDED, &e );
        }

        void EntityManager::RemoveComponent(Entity *entity, ComponentTypeID id)
        {
        #if defined(CONFIG_DEBUG)

            UAssert( id != GetComponentID( Transform ),
                "You can't remove a Transform component." );

        #endif

            removeComponent( entity, id, true );
        }

        Component *EntityManager::GetComponent(const Entity *entity, ComponentTypeID id) const
        {
            // he doesn't have this...
            if (!entity->HasComponent( 1ull << id ))
                return nullptr;

            return m_componentTypes[ id ][ entity->m_id ];
        }

        ComponentVector EntityManager::GetComponents(const Entity *entity) const
        {
            ComponentVector found;

            // somethings silly
            if (!entity)
                return found;

            const auto entityID = entity->m_id;

            for (uint32 i = 0; i < m_componentTypes.size( ); ++i)
            {
                if (entity->HasComponent( 1ull << i ))
                    found.emplace_back( m_componentTypes[ i ][ entityID ] );
            }

            return found;
        }

        Component *EntityManager::GetComponentInChildren(const Entity* entity, ComponentTypeID id) const
        {
            std::queue<const std::vector<EntityID>*> childrenContainer;

            childrenContainer.push( m_hierarchy.GetChildren( entity ) );

            while (childrenContainer.size( ) > 0)
            {
                auto &children = *childrenContainer.front( );

                for (auto &child : children)
                {
                    auto childEntity = &m_cache[ child ];
                    auto component = GetComponent( childEntity, id );

                    if (component)
                        return component;

                    childrenContainer.push( m_hierarchy.GetChildren( childEntity )) ;
                }

                childrenContainer.pop( );
            }

            return nullptr;
        }

        Component *EntityManager::GetComponentInParent(const Entity* entity, ComponentTypeID id) const
        {
            auto parentID = m_hierarchy.GetParent( entity );

            if (parentID == -1)
                return nullptr;

            auto parent = &m_cache[ parentID ];

            return GetComponent( parent, id );
        }

        ComponentVector EntityManager::GetComponentsInChildren(const Entity* entity, ComponentTypeID id) const
        {
            ComponentVector components;
            std::queue<const std::vector<EntityID>*> childrenContainer;

            childrenContainer.push( m_hierarchy.GetChildren( entity ) );

            while (childrenContainer.size( ) > 0)
            {
                auto &children = *childrenContainer.front( );

                for (auto &child : children)
                {
                    auto childEntity = &m_cache[ child ];
                    auto component = GetComponent( childEntity, id );

                    if (component)
                        components.push_back( component );

                    childrenContainer.push( m_hierarchy.GetChildren( childEntity ) );
                }

                childrenContainer.pop( );
            }

            return components;
        }

        ComponentVector EntityManager::GetComponentsInParents(const Entity* entity, ComponentTypeID id) const
        {
            ComponentVector components;
            auto parentID = m_hierarchy.GetParent( entity );

            while (parentID != -1)
            {
                auto parent = &m_cache[ parentID ];

                auto component = GetComponent( parent, id );

                if (component)
                    components.push_back( component );

                parentID = m_hierarchy.GetParent( parent );
            }

            return components;
        }

        uint EntityManager::GetSiblingIndex(const Entity *entity) const
        {
            return m_hierarchy.GetSiblingIndex( entity );
        }

        void EntityManager::SetAsFirstSibling(const Entity *entity)
        {
            m_hierarchy.SetAsFirstSibling( entity );
        }

        void EntityManager::SetSiblingIndex(const Entity *entity, uint index)
        {
            m_hierarchy.SetSiblingIndex( entity, index );
        }
         
        EntityVector EntityManager::GetEntities(const Filter &filter) const
        {
            EntityVector found;

            for (uint i = 0; i < m_active.size( ); ++i)
            {
                auto *entity = m_active[ i ];

                if (entity && filter.Matches( entity ))
                    found.push_back( entity );
            }

            return found;
        }

        Entity *EntityManager::GetEntity(EntityID id)
        {
            // out of bounds
            if (id + 1u > m_cache.size( ))
                return nullptr;

            return &m_cache[ id ];
        }

        Entity *EntityManager::GetEntityUnique(EntityUniqueID id) const
        {
            auto found = m_unique.find( id );

            // nullptr if not found
            return (found == m_unique.end( )) ? nullptr : found->second;
        }

        void EntityManager::BeforeRemove(Entity *entity)
        {
            EntityEventArgs e( WORLD_ENTITY_REMOVED, entity );

            // we're removing man
            m_world->Dispatch( WORLD_ENTITY_REMOVED, &e );
        }

        void EntityManager::Remove(Entity *entity)
        {
            // not active, so we don't want to delete him
            if (!entity->IsActive( ))
                return;

            // Remove the children before the parent is removed
            auto children = m_hierarchy.GetChildren( entity );

            while (children->size( ) > 0)
            {
                auto &child = ( *children )[ 0 ];
                Remove( &m_cache[ child ] );
            }

            m_hierarchy.RemoveEntity( entity );

            clearComponents( entity, true );

            entity->m_active = false;

            m_active.erase( find( m_active.begin( ), m_active.end( ), entity ) );

            m_unique.erase( entity->m_uniqueID );

            m_inactive.push_back( entity );
        }

        Entity::EventDispatcher &EntityManager::GetEntityEvents(Entity *entity)
        {
            return m_events[ entity->m_id ];
        }

        bool EntityManager::CompareComponents(const Component *a, const Component *b)
        {
            // note: descending order
            return b->m_uniqueID < a->m_uniqueID;
        }

        ////////////////////////////////////////////////////////////////////////
        // Private Methods
        ////////////////////////////////////////////////////////////////////////

        Entity *EntityManager::create(void)
        {
            Entity *entity = nullptr;

            EntityUniqueID uniqueID = m_nextEntityUID++;

            // we can't use the available queue
            if (m_inactive.empty( ))
            {
                m_cache.emplace_back( m_world, m_nextEntityID++ );

                entity = &m_cache.back( );

                // add a new event listener
                m_events.emplace_back( );
            }
            // we can use the queue so just reset and pop the last one
            else
            {
                entity = m_inactive.back( );

                m_inactive.pop_back( );

                entity->reset( );

                GetEntityEvents( entity ).ClearHandlers( );
            }

            m_active.push_back( entity );

            entity->m_uniqueID = uniqueID;

            m_unique[ uniqueID ] = entity;

            m_hierarchy.AddEntity( entity );

            return entity;
        }

        void EntityManager::dispatchCreated(Entity *entity)
        {
            EntityEventArgs e( WORLD_ENTITY_ADDED, entity );

            m_world->Dispatch( WORLD_ENTITY_ADDED, &e );

            auto &children = *m_hierarchy.GetChildren( entity );

            // dispatch children AFTER dispatching parent
            for (auto childID : children)
                dispatchCreated( GetEntity( childID ) );
        }

        void EntityManager::addComponent(Entity *entity, Component *component)
        {
            auto componentID = component->GetTypeID( );

            auto &components = m_componentTypes[ componentID ];

            const auto entityID = entity->m_id;

            // entity ids are zero based
            if (entityID + 1u > components.size( ))
                components.resize( (entityID + 1u) * 2 );

            components[ entityID ] = component;

            entity->setType( 1ull << componentID );

            component->m_uniqueID = m_nextComponentUID++;
            component->m_owner = entity;
        }

        void EntityManager::removeComponent(Entity *entity, ComponentTypeID id, bool dispatch)
        {
            const ComponentTypeMask mask = 1ull << id;

            // he doesn't have this...
            if (!entity->HasComponent( mask ))
                return;

            auto oldMask = entity->m_typeMask;

            entity->unsetType( mask );

            auto *&component = m_componentTypes[ id ][ entity->m_id ];

            if (dispatch)
            {
                ComponentRemovedEventArgs e( WORLD_ENTITY_COMPONENT_REMOVED, entity, component, oldMask );

                m_world->Dispatch( WORLD_ENTITY_COMPONENT_REMOVED, &e );
            }

            delete component;

            component = nullptr;
        }

        void EntityManager::initializeComponents(Entity *entity)
        {
            const auto size = m_componentTypes.size( );
            const auto id = entity->m_id;

            ComponentEventArgs args( WORLD_ENTITY_COMPONENT_ADDED, entity, nullptr );

            // transform can be assumed to be the first component type
            for (uint32 i = 0; i < size; ++i)
            {
                if (entity->HasComponent( 1ull << i ))
                {
                    auto *component = m_componentTypes[ i ][ id ];

                    component->OnInitialize( );

                    args.component = component;

                    m_world->Dispatch( WORLD_ENTITY_COMPONENT_ADDED, &args );
                }
            }
        }

        void EntityManager::clearComponents(Entity *entity, bool dispatch)
        {
            const auto size = m_componentTypes.size( );
            const auto entityID = entity->m_id;

            // components to remove
            ComponentVector toRemove;

            for (ComponentTypeID i = 0; i < size; ++i)
            {
                // if the entity has this component, sort insert it into the 
                // the queue to remove (based on instance id)
                // this is so components with dependencies are deleted in the correct order
                if (entity->HasComponent( 1ull << i ))
                    utils::InsertionSort( toRemove, m_componentTypes[ i ][ entityID ], CompareComponents );
            }

            auto const removeCount = toRemove.size( );

            if (dispatch)
            {
                ComponentEventArgs args( WORLD_ENTITY_COMPONENT_REMOVED, entity, nullptr );

                for (uint32 i = 0; i < removeCount; ++i)
                {
                    args.component = toRemove[ i ];

                    m_world->Dispatch( WORLD_ENTITY_COMPONENT_REMOVED, &args );
                }
            }

            for (uint32 i = 0; i < removeCount; ++i) 
            {
                auto *component = toRemove[ i ];

                // reference to the pointer of this component for this entity
                auto *&componentReference = m_componentTypes[ component->m_typeID ][ entityID ];

                delete component;

                // make sure the value in the container is set to null
                componentReference = nullptr;
            }
        }
    }
}
