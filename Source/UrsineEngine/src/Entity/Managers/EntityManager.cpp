/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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

namespace ursine
{
    namespace ecs
    {
        ////////////////////////////////////////////////////////////////////////
        // Constructors/Destructors
        ////////////////////////////////////////////////////////////////////////

        EntityManager::EntityManager(World *world)
            : WorldManager( world )
              , EventDispatcher( this )
              , m_nextEntityID( 0 )
              , m_nextEntityUID( 0 )
              , m_nextComponentUID( 0 )
              , m_dataPath( "" )
        {
        }

        EntityManager::~EntityManager(void)
        {
            m_serializer.ClearCache( );

            for (auto entity : m_active)
                clearComponents( entity );
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

        Entity *EntityManager::Create(const std::string &identifier)
        {
            auto entity = create( );

            m_serializer.Deserialize( identifier, this, entity );

            auto transform = entity->GetComponent<Transform>( );

            // add the transform if it doesn't already exist
            entity->m_transform = transform ? transform :
                                      entity->AddComponent<Transform>( );

            initializeComponents( entity );

            dispatchCreated( entity );

            return entity;
        }

        Entity *EntityManager::Create(const std::string &identifier, const Json &merge)
        {
            auto entity = create( );

            m_serializer.Deserialize( identifier, merge, this, entity );

            auto transform = entity->GetComponent<Transform>( );

            // add the transform if it doesn't already exist
            entity->m_transform = transform ? transform :
                                      new Transform( ); // <-- change this later

            initializeComponents( entity );

            dispatchCreated( entity );

            return entity;
        }

        void EntityManager::AddComponent(Entity *entity, Component *component)
        {
            UAssert(!entity->HasComponent(component->GetTypeMask()),
                "Component already exists: %s",
                component->GetName().c_str())            ;

            addComponent( entity, component );

            component->OnInitialize( );

            ComponentEventArgs e( EM_COMPONENT_ADDED, entity, component );

            Dispatch( EM_COMPONENT_ADDED, &e );
        }

        void EntityManager::RemoveComponent(Entity *entity, ComponentTypeID id)
        {
#ifdef CONFIG_DEBUG
            UAssert(id != GetComponentID(Transform),
                "You can't remove a Transform component.");
#endif

            removeComponent( entity, id, true );
        }

        Component *EntityManager::GetComponent(const Entity *entity, ComponentTypeID id) const
        {
            // he doesn't have this...
            if (!entity->HasComponent( 1ull << id ))
                return nullptr;

            return _component_types[ id ][ entity->m_id ];
        }

        ComponentVector EntityManager::GetComponents(const Entity *entity) const
        {
            ComponentVector found;

            // somethings silly
            if (!entity)
                return found;

            const auto entity_id = entity->m_id;

            for (uint32 i = 0; i < _component_types.size( ); ++i)
            {
                auto &components = _component_types[ i ];

                Component *component;

                if (entity_id + 1u <= components.size( ) &&
                    (component = components[ entity_id ]) != nullptr)
                {
                    found.push_back( component );
                }
            }

            return found;
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
            EntityEventArgs e( EM_ENTITY_REMOVED, entity );

            // we're removing man
            Dispatch( EM_ENTITY_REMOVED, &e );
        }

        void EntityManager::Remove(Entity *entity)
        {
            // not active, so we don't want to delete him
            if (!entity->IsActive( ))
                return;

            clearComponents( entity, true );

            utils::FlagUnset( entity->m_flags, Entity::ACTIVE );

            m_active.erase( find( m_active.begin( ), m_active.end( ), entity ) );

            m_unique.erase( entity->m_uniqueID );

            m_inactive.push_back( entity );
        }

        Entity::EventDispatcher &EntityManager::GetEntityEvents(Entity *entity)
        {
            return m_events[ entity->m_id ];
        }

        Json EntityManager::Serialize(Entity *entity) const
        {
            return m_serializer.Serialize( entity );
        }

        const std::string &EntityManager::GetDataPath(void) const
        {
            return m_dataPath;
        }

        void EntityManager::SetDataPath(const std::string &path)
        {
            m_dataPath = path;
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

            EntityUniqueID unique_id = m_nextEntityUID++;

            // we can't use the available queue
            if (m_inactive.empty( ))
            {
                m_cache.emplace_back( _world, m_nextEntityID++ );

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

            entity->m_uniqueID = unique_id;

            m_unique[ unique_id ] = entity;

            return entity;
        }

        void EntityManager::dispatchCreated(Entity *entity)
        {
            EntityEventArgs e( EM_ENTITY_ADDED, entity );

            Dispatch( EM_ENTITY_ADDED, &e );
        }

        void EntityManager::addComponent(Entity *entity, Component *component)
        {
            auto component_id = component->GetTypeID( );

            ComponentVector &components = _component_types[ component_id ];

            const auto entity_id = entity->m_id;

            // entity ids are zero based
            if (entity_id + 1u > components.size( ))
                components.resize( (entity_id + 1u) * 2 );

            components[ entity_id ] = component;

            entity->setType( 1ull << component_id );

            component->m_uniqueID = m_nextComponentUID++;
            component->m_owner = entity;
        }

        void EntityManager::removeComponent(Entity *entity, ComponentTypeID id, bool dispatch)
        {
            const ComponentTypeMask mask = 1ull << id;

            // he doesn't have this...
            if (!entity->HasComponent( mask ))
                return;

            entity->unsetType( mask );

            Component *&component = _component_types[ id ][ entity->m_id ];

            if (dispatch)
            {
                ComponentEventArgs e( EM_COMPONENT_REMOVED, entity, component );

                Dispatch( EM_COMPONENT_REMOVED, &e );
            }

            delete component;

            component = nullptr;
        }

        void EntityManager::initializeComponents(Entity *entity)
        {
            const auto size = _component_types.size( );
            const auto id = entity->m_id;

            ComponentEventArgs args( EM_COMPONENT_ADDED, entity, nullptr );

            // transform can be assumed to be the first component type
            for (uint32 i = 0; i < size; ++i)
            {
                if (entity->HasComponent( 1ull << i ))
                {
                    auto *component = _component_types[ i ][ id ];

                    component->OnInitialize( );

                    args.component = component;

                    Dispatch( EM_COMPONENT_ADDED, &args );
                }
            }
        }

        void EntityManager::clearComponents(Entity *entity, bool dispatch)
        {
            const auto size = _component_types.size( );
            const auto id = entity->m_id;

            // components to remove
            ComponentVector to_remove;

            URSINE_TODO("optimize this once entity parenting is implemented: start from the deepest child and clear components going up the tree.")
            ;

            for (ComponentTypeID i = 0; i < size; ++i)
            {
                // if the entity has this component, sort insert it into the 
                // the queue to remove (based on instance id)
                // this is so components with dependencies are deleted in the correct order
                if (entity->HasComponent( 1ull << i ))
                    utils::InsertionSort( to_remove, _component_types[ i ][ id ], CompareComponents );
            }

            auto const remove_count = to_remove.size( );

            if (dispatch)
            {
                ComponentEventArgs args( EM_COMPONENT_REMOVED, entity, nullptr );

                for (uint32 i = 0; i < remove_count; ++i)
                {
                    args.component = to_remove[ i ];

                    Dispatch( EM_COMPONENT_REMOVED, &args );
                }
            }

            for (uint32 i = 0; i < remove_count; ++i)
                delete to_remove[ i ];
        }
    }
}
