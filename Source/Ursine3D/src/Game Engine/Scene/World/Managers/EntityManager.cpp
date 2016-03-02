/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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
#include "EntitySerializer.h"
#include "EntityEvent.h"
#include "EntityHandle.h"

#include <queue>

#if defined(URSINE_WITH_EDITOR)

#include "Notification.h"

#endif

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
                    auto setter = derived.GetStaticMethod( "SetComponentID", { typeof( ComponentTypeID ) } );

                    UAssert( setter.IsValid( ),
                        "Native component '%s' doesn't have a static method SetComponentID.\n"
                        "Most likely missing NATIVE_COMPONENT in declaration",
                        derived.GetName( ).c_str( )
                    );

                    UAssert( nextID < kMaxComponentCount, "We're maxed out son." );

                    setter.Invoke( nextID++ );

                    auto &defaultCtor = derived.GetDynamicConstructor( );

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
            , m_nextComponentUID( 0 )
        {
            configureComponents( );
        }

        EntityManager::~EntityManager(void)
        {
            while (m_active.size( ) > 0)
            {
                auto id = m_active[ 0 ];

                Remove( getEntityByID( id ) );
            }
        }

        ////////////////////////////////////////////////////////////////////////
        // Public Methods
        ////////////////////////////////////////////////////////////////////////

        EntityHandle EntityManager::Create(void)
        {
            auto entity = create( );

            entity->m_transform = entity->AddComponent<Transform>( );

            dispatchCreated( entity );

            return entity;
        }
         
        EntityHandle EntityManager::Clone(const EntityHandle &entity)
        {
            URSINE_TODO( "optimize by skipping the serialization step" );

            EntitySerializer serializer;

            auto data = serializer.SerializeArchetype( entity );

            return serializer.DeserializeArchetype( m_world, data );
        }

        EntityHandleVector EntityManager::GetRootEntities(void)
        {
            auto &children = *m_hierarchy.GetRootNode( ).Children( );

            EntityHandleVector entities;

            for (auto child : children)
                entities.emplace_back( GetEntityByID( child ) );

            return entities;
        }

        EntityHandleVector EntityManager::GetActiveEntities(void) const
        {
            EntityHandleVector entities;

            for (auto id : m_active)
                entities.emplace_back( GetEntityByID( id ) );

            return entities;
        }

        void EntityManager::AddComponent(const EntityHandle &entity, Component *component)
        {
            UAssert( !entity->HasComponent( component->GetTypeMask( ) ),
                "Component already exists: %s",
                component->GetType( ).GetName( ).c_str( ) );

        #if defined(URSINE_WITH_EDITOR)

            auto type = component->GetType( );
            auto *required = type.GetMeta( ).GetProperty<RequiresComponents>( );

            if (required)
            {
                for (auto &componentType : required->componentTypes)
                {
                    auto id = componentType.GetStaticField( "ComponentID" );
                    
                    UAssert( id.IsValid( ),
                        "Required component '%s' doesn't have ComponentID static field.",
                        componentType.GetName( ).c_str( )
                    );

                    ComponentTypeMask mask;

                    mask.set( id.GetValue( ).GetValue<ComponentTypeID>( ), true );

                    if (!entity->HasComponent( mask ))
                    {
                        NotificationConfig error;

                        error.type = NOTIFY_ERROR;
                        error.header = "Error";
                        error.message = 
                            "Component <strong class=\"highlight\">" + type.GetName( ) + "</strong> requires component "+ 
                            "<strong class=\"highlight\">" + componentType.GetName( ) + "</strong>";

                        error.buttons = {
                            { 
                                "Add Dependency", 
                                [=](Notification &notification)
                                {
                                    notification.Close( );

                                    entity->AddComponent( componentType.CreateDynamic( ).GetValue<Component*>( ) );
                                }
                            }
                        };

                        EditorPostNotification( error );

                        // this is so we can ensure dependencies in the deconstructor
                        component->m_owner = entity;

                        // we ain't want this guy
                        delete component;

                        return;
                    }
                }
            }

        #endif

            addComponent( entity, component );

            component->onInitialize( );

            auto *scene = m_world->GetOwner( );

            // determine if the scene is already configured
            if (scene != nullptr)
                component->onSceneReady( scene );

            ComponentEventArgs e( 
                WORLD_ENTITY_COMPONENT_ADDED, 
                entity,
                component 
            );

            m_world->Dispatch( WORLD_ENTITY_COMPONENT_ADDED, &e );
        }

        void EntityManager::RemoveComponent(const EntityHandle &entity, ComponentTypeID id)
        {
        #if defined(CONFIG_DEBUG)

            UAssert( id != GetComponentID( Transform ),
                "You can't remove a Transform component." );

        #endif

        #if defined(URSINE_WITH_EDITOR)

            ComponentTypeMask mask;

            mask.set( id, true );

            // the entity doesn't have this component
            if (!entity->HasComponent( mask ))
                return;

            auto *instance = m_componentTypes[ id ][ entity->m_id ];

            // the component removed elsewhere
            if (!instance)
                return;

            auto removedType = instance->GetType( );
            auto components = GetComponents( entity.Get( ) );

            for (auto *component : components)
            {
                auto type = component->GetType( );

                auto *required = type.GetMeta( ).GetProperty<RequiresComponents>( );

                if (required)
                {
                    auto search = required->componentTypes.Find( removedType );

                    if (search != required->componentTypes.end( ))
                    {
                        NotificationConfig error;

                        error.type = NOTIFY_ERROR;
                        error.header = "Error";
                        error.message = 
                            "Component <strong class=\"highlight\">" + type.GetName( ) + "</strong> requires component " + 
                            "<strong class=\"highlight\">" + removedType.GetName( ) + "</strong>";

                        EditorPostNotification( error );

                        return;
                    }
                }
            }

        #endif

            removeComponent( entity, id, true );
        }

        Component *EntityManager::GetComponent(const Entity *entity, ComponentTypeID id) const
        {
            ComponentTypeMask mask;

            mask.set( id, true );

            // he doesn't have this...
            if (!entity->HasComponent( mask ))
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
                ComponentTypeMask mask;

                mask.set( i, true );

                if (entity->HasComponent( mask ))
                    found.emplace_back( m_componentTypes[ i ][ entityID ] );
            }

            return found;
        }

        const std::vector<EntityID> *EntityManager::GetChildren(const Entity *entity) const
        {
            return m_hierarchy.GetChildren( entity->m_id );
        }

        Component *EntityManager::GetComponentInChildren(const Entity *entity, ComponentTypeID id) const
        {
            std::queue<const std::vector<EntityID>*> childrenContainer;

            childrenContainer.push( m_hierarchy.GetChildren( entity->m_id ) );

            while (childrenContainer.size( ) > 0)
            {
                auto &children = *childrenContainer.front( );

                for (auto &child : children)
                {
                    auto childEntity = &m_cache[ child ];
                    auto component = GetComponent( childEntity, id );

                    if (component)
                        return component;

                    childrenContainer.push( m_hierarchy.GetChildren( childEntity->m_id ) );
                }

                childrenContainer.pop( );
            }

            return nullptr;
        }

        Component *EntityManager::GetComponentInParent(const Entity *entity, ComponentTypeID id) const
        {
            const auto *parent = entity;
            Component *component = nullptr;

            do
            {
                auto parentID = m_hierarchy.GetParent( parent->m_id );

                if (parentID == -1)
                    return nullptr;

                parent = &m_cache[ parentID ];
                
                component = GetComponent( parent, id );

            // was desired component found
            } while (component == nullptr);

            return component;
        }

        ComponentVector EntityManager::GetComponentsInChildren(const Entity* entity, ComponentTypeID id) const
        {
            ComponentVector components;
            std::queue<const std::vector<EntityID>*> childrenContainer;

            childrenContainer.push( m_hierarchy.GetChildren( entity->m_id ) );

            while (childrenContainer.size( ) > 0)
            {
                auto &children = *childrenContainer.front( );

                for (auto &child : children)
                {
                    auto childEntity = &m_cache[ child ];
                    auto component = GetComponent( childEntity, id );

                    if (component)
                        components.push_back( component );

                    childrenContainer.push( m_hierarchy.GetChildren( child ) );
                }

                childrenContainer.pop( );
            }

            return components;
        }

        ComponentVector EntityManager::GetComponentsInParents(const Entity* entity, ComponentTypeID id) const
        {
            ComponentVector components;

            auto parentID = m_hierarchy.GetParent( entity->m_id );

            while (parentID != -1)
            {
                auto parent = &m_cache[ parentID ];

                auto component = GetComponent( parent, id );

                if (component)
                    components.push_back( component );

                parentID = m_hierarchy.GetParent( parent->m_id );
            }

            return components;
        }

        uint EntityManager::GetSiblingIndex(const Entity *entity) const
        {
            return m_hierarchy.GetSiblingIndex( entity->m_id );
        }

        void EntityManager::SetAsFirstSibling(const Entity *entity)
        {
            m_hierarchy.SetAsFirstSibling( entity->m_id );
        }

        void EntityManager::SetSiblingIndex(const Entity *entity, uint index)
        {
            m_hierarchy.SetSiblingIndex( entity->m_id, index );
        }
         
        EntityHandleVector EntityManager::GetEntities(const Filter &filter) const
        {
            EntityHandleVector found;

            for (uint i = 0; i < m_active.size( ); ++i)
            {
                auto *entity = getEntityByID( m_active[ i ] );

                if (entity && filter.Matches( entity ))
                    found.emplace_back( EntityHandle( entity ) );
            }

            return found;
        }

        Entity *EntityManager::GetEntity(const EntityHandle &handle) const
        {
            auto *entity = getEntityByID( handle.m_id );

            // mismatching versions mean this handle is invalid
            return entity && entity->m_version == handle.m_version ? entity : nullptr;
        }

        EntityHandle EntityManager::GetEntityByID(EntityID id) const
        {
            return getEntityByID( id );
        }

        void EntityManager::BeforeRemove(const EntityHandle &entity)
        {
            // for each child, remove it
            for (auto child : entity->GetTransform( )->GetChildren( ))
                BeforeRemove( child->GetOwner( ).Get( ) );

            EntityEventArgs e( 
                WORLD_ENTITY_REMOVED, 
                entity
            );

            // we're removing man
            entity->Dispatch( ENTITY_REMOVED, EventArgs::Empty );

            m_world->Dispatch( WORLD_ENTITY_REMOVED, &e );
        }

        void EntityManager::Remove(const EntityHandle &entity)
        {
            // not active, so we don't want to delete him
            if (!entity->IsActive( ))
                return;

            auto entityID = entity->m_id;

            // Remove the children before the parent is removed
            auto children = m_hierarchy.GetChildren( entityID );

            while (children->size( ) > 0)
            {
                auto &child = (*children)[ 0 ];

                Remove( &m_cache[ child ] );
            }

            m_hierarchy.RemoveEntity( entity );

            clearComponents( entity, true );

            entity->m_active = false;

            m_active.erase( find( m_active.begin( ), m_active.end( ), entityID ) );

            m_inactive.push_back( entityID );
        }

        Entity::EventDispatcher &EntityManager::GetEntityEvents(Entity *entity)
        {
            return m_events[ entity->m_id ];
        }

        bool EntityManager::CompareComponentsAscending(const Component *a, const Component *b)
        {
            return a->m_uniqueID < b->m_uniqueID;
        }

        bool EntityManager::CompareComponentsDescending(const Component *a, const Component *b)
        {
            return b->m_uniqueID < a->m_uniqueID;
        }

        ////////////////////////////////////////////////////////////////////////
        // Private Methods
        ////////////////////////////////////////////////////////////////////////

        void EntityManager::initializeScene(void)
        {
            for (auto id : m_active)
                initializeComponentsForScene( getEntityByID( id ) );
        }

        Entity *EntityManager::getEntityByID(EntityID id) const
        {
            if (id >= m_cache.size( ))
                return nullptr;

            return const_cast<Entity*>( &m_cache[ id ] );
        }

        EntityHandle EntityManager::create(void)
        {
            Entity *entity = nullptr;

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
                entity = getEntityByID( m_inactive.back( ) );

                m_inactive.pop_back( );

                entity->reset( );

                GetEntityEvents( entity ).ClearHandlers( );
            }

            m_active.push_back( entity->m_id );

            m_hierarchy.AddEntity( entity );

            return entity;
        }

        void EntityManager::dispatchCreated(const EntityHandle &entity)
        {
            EntityEventArgs e( WORLD_ENTITY_ADDED, entity );

            m_world->Dispatch( WORLD_ENTITY_ADDED, &e );

            auto &children = *m_hierarchy.GetChildren( entity->m_id );

            // dispatch children AFTER dispatching parent
            for (auto childID : children)
                dispatchCreated( GetEntityByID( childID ) );
        }

        void EntityManager::addComponent(const EntityHandle &entity, Component *component)
        {
            auto componentID = component->GetTypeID( );

            auto &components = m_componentTypes[ componentID ];

            const auto entityID = entity->m_id;

            // entity ids are zero based
            if (entityID >= components.size( ))
                components.resize( (entityID + 1u) * 2 );

            components[ entityID ] = component;

            ComponentTypeMask mask;

            mask.set( componentID, true );

            entity->setType( mask );

            component->m_uniqueID = m_nextComponentUID++;
            component->m_owner = entity;
        }

        void EntityManager::removeComponent(const EntityHandle &entity, ComponentTypeID id, bool dispatch)
        {
            ComponentTypeMask mask;

            mask.set( id, true );

            // he doesn't have this...
            if (!entity->HasComponent( mask ))
                return;

            auto oldMask = entity->m_typeMask;

            entity->unsetType( mask );

            auto *&component = m_componentTypes[ id ][ entity->m_id ];

            if (dispatch)
            {
                ComponentRemovedEventArgs e( 
                    WORLD_ENTITY_COMPONENT_REMOVED, 
                    entity,
                    component, 
                    oldMask 
                );

                m_world->Dispatch( WORLD_ENTITY_COMPONENT_REMOVED, &e );
            }

            delete component;

            component = nullptr;
        }

        void EntityManager::initializeComponents(const EntityHandle &entity)
        {
            const auto size = m_componentTypes.size( );
            const auto id = entity->m_id;

            ComponentEventArgs args( 
                WORLD_ENTITY_COMPONENT_ADDED, 
                entity, 
                nullptr 
            );

            for (uint32 i = 0; i < size; ++i)
            {
                ComponentTypeMask mask;

                mask.set( i, true );

                if (entity->HasComponent( mask ))
                {
                    auto *component = m_componentTypes[ i ][ id ];

                    component->onInitialize( );

                    args.component = component;

                    m_world->Dispatch( WORLD_ENTITY_COMPONENT_ADDED, &args );
                }
            }
        }

        void EntityManager::initializeComponentsForScene(const EntityHandle &entity)
        {
            const auto size = m_componentTypes.size( );
            const auto id = entity->m_id;

            auto *scene = m_world->GetOwner( );

            for (uint32 i = 0; i < size; ++i)
            {
                ComponentTypeMask mask;

                mask.set( i, true );

                if (entity->HasComponent( mask ))
                {
                    auto *component = m_componentTypes[ i ][ id ];

                    component->onSceneReady( scene );
                }
            }
        }

        void EntityManager::clearComponents(const EntityHandle &entity, bool dispatch)
        {
            const auto size = m_componentTypes.size( );
            const auto entityID = entity->m_id;

            // components to remove
            ComponentVector toRemove;

            for (ComponentTypeID i = 0; i < size; ++i)
            {
                ComponentTypeMask mask;

                mask.set( i, true );

                // if the entity has this component, sort insert it into the 
                // the queue to remove (based on instance id)
                // this is so components with dependencies are deleted in the correct order
                if (entity->HasComponent( mask ))
                    utils::InsertionSort( toRemove, m_componentTypes[ i ][ entityID ], CompareComponentsDescending );
            }

            auto const removeCount = toRemove.size( );

            if (dispatch)
            {
                ComponentEventArgs args( 
                    WORLD_ENTITY_COMPONENT_REMOVED, 
                    entity,
                    nullptr 
                );

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
