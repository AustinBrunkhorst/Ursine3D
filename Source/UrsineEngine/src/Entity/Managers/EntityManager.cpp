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

namespace Ursine
{
    namespace ECS
    {
        ////////////////////////////////////////////////////////////////////////
        // Constructors/Destructors
        ////////////////////////////////////////////////////////////////////////

        EntityManager::EntityManager(World *world)
            : WorldManager( world )
            , EventDispatcher( this )
            , _next_entity_id( 0 )
            , _next_entity_uid( 0 )
            , _next_component_uid( 0 )
            , _data_path( "" ) { }

        EntityManager::~EntityManager(void)
        {
            _serializer.ClearCache( );

            for (auto entity : _active)
                clearComponents( entity );
        }

        ////////////////////////////////////////////////////////////////////////
        // Public Methods
        ////////////////////////////////////////////////////////////////////////

        Entity *EntityManager::Create(void)
        {
            auto entity = create( );

            entity->_transform = entity->AddComponent<Transform>( );

            dispatchCreated( entity );

            return entity;
        }

        Entity *EntityManager::Create(const std::string &identifier)
        {
            auto entity = create( );

            _serializer.Deserialize( identifier, this, entity );

            auto transform = entity->GetComponent<Transform>( );

            // add the transform if it doesn't already exist
            entity->_transform = transform ? transform :
                entity->AddComponent<Transform>( );

            initializeComponents( entity );

            dispatchCreated( entity );

            return entity;
        }

        Entity *EntityManager::Create(const std::string &identifier, const Json &merge)
        {
            auto entity = create( );

            _serializer.Deserialize( identifier, merge, this, entity );

            auto transform = entity->GetComponent<Transform>( );

            // add the transform if it doesn't already exist
            entity->_transform = transform ? transform :
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

            return _component_types[ id ][ entity->_id ];
        }

        ComponentVector EntityManager::GetComponents(const Entity *entity) const
        {
            ComponentVector found;

            // somethings silly
            if (!entity)
                return found;

            const auto entity_id = entity->_id;

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

            for (uint i = 0; i < _active.size( ); ++i)
            {
                auto *entity = _active[ i ];

                if (entity && filter.Matches( entity ))
                    found.push_back( entity );
            }

            return found;
        }

        Entity *EntityManager::GetEntity(EntityID id)
        {
            // out of bounds
            if (id + 1u > _cache.size( ))
                return nullptr;

            return &_cache[ id ];
        }

        Entity *EntityManager::GetEntityUnique(EntityUniqueID id) const
        {
            auto found = _unique.find( id );

            // nullptr if not found
            return (found == _unique.end( )) ? nullptr : found->second;
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

            Utils::FlagUnset( entity->_flags, Entity::ACTIVE );

            _active.erase( find( _active.begin( ), _active.end( ), entity ) );

            _unique.erase( entity->_unique_id );

            _inactive.push_back( entity );
        }

        Entity::EventDispatcher &EntityManager::GetEntityEvents(Entity *entity)
        {
            return _events[ entity->_id ];
        }

        Json EntityManager::Serialize(Entity *entity) const
        {
            return _serializer.Serialize( entity );
        }

        const std::string &EntityManager::GetDataPath(void) const
        {
            return _data_path;
        }

        void EntityManager::SetDataPath(const std::string &path)
        {
            _data_path = path;
        }

        bool EntityManager::CompareComponents(const Component *a, const Component *b)
        {
            // note: descending order
            return b->_unique_id < a->_unique_id;
        }

        ////////////////////////////////////////////////////////////////////////
        // Private Methods
        ////////////////////////////////////////////////////////////////////////

        Entity *EntityManager::create(void)
        {
            Entity *entity = nullptr;

            EntityUniqueID unique_id = _next_entity_uid++;

            // we can't use the available queue
            if (_inactive.empty( ))
            {
                _cache.emplace_back( _world, _next_entity_id++ );

                entity = &_cache.back( );

                // add a new event listener
                _events.emplace_back( );
            }
            // we can use the queue so just reset and pop the last one
            else
            {
                entity = _inactive.back( );

                _inactive.pop_back( );

                entity->reset( );

                GetEntityEvents( entity ).ClearHandlers( );
            }

            _active.push_back( entity );

            entity->_unique_id = unique_id;

            _unique[ unique_id ] = entity;

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

            const auto entity_id = entity->_id;

            // entity ids are zero based
            if (entity_id + 1u > components.size( ))
                components.resize( (entity_id + 1u) * 2 );

            components[ entity_id ] = component;

            entity->setType( 1ull << component_id );

            component->_unique_id = _next_component_uid++;
            component->_owner = entity;
        }

        void EntityManager::removeComponent(Entity *entity, ComponentTypeID id, bool dispatch)
        {
            const ComponentTypeMask mask = 1ull << id;

            // he doesn't have this...
            if (!entity->HasComponent( mask ))
                return;

            entity->unsetType( mask );

            Component *&component = _component_types[ id ][ entity->_id ];

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
            const auto id = entity->_id;

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
            const auto id = entity->_id;

            // components to remove
            ComponentVector to_remove;

            URSINE_TODO("optimize this once entity parenting is implemented: start from the deepest child and clear components going up the tree.");

            for (ComponentTypeID i = 0; i < size; ++i)
            {
                // if the entity has this component, sort insert it into the 
                // the queue to remove (based on instance id)
                // this is so components with dependencies are deleted in the correct order
                if (entity->HasComponent( 1ull << i ))
                    Utils::InsertionSort( to_remove, _component_types[ i ][ id ], CompareComponents );
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
