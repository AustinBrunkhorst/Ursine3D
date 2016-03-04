/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** World.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/


#include "UrsinePrecompiled.h"

#include "World.h"

#include "Entity.h"

#include "EntityManager.h"
#include "NameManager.h"
#include "UtilityManager.h"
#include "SystemManager.h"

#include "EntitySerializer.h"
#include "WorldSerializer.h"

#include "WorldConfigComponent.h"
#include "WorldData.h"

#include "EntityHandle.h"

#include "ArchetypeData.h"

namespace
{
    const auto kWorldSettingsEntityName = "World Settings";
}

namespace ursine
{
    namespace ecs
    {
        World::World(void)
            : EventDispatcher( this )
            , m_entityManager( nullptr )
            , m_systemManager( nullptr )
            , m_nameManager( nullptr )
            , m_utilityManager( nullptr )
            , m_owner( nullptr )
        {
            m_entityManager = new EntityManager( this );
            m_entityManager->OnInitialize( );

            m_nameManager = new NameManager( this );
            m_nameManager->OnInitialize( );

            m_utilityManager = new UtilityManager( this );
            m_utilityManager->OnInitialize( );

            // ensure the settings entity is created before the system manager
            m_settings = CreateEntity( kWorldSettingsEntityName );
            m_settings->EnableDeletion( false );
            m_settings->EnableHierarchyChange( false );

            m_systemManager = new SystemManager( this );
            m_systemManager->OnInitialize( );
        }

        World::~World(void)
        {
            delete m_systemManager;

            m_systemManager = nullptr;

            delete m_utilityManager;

            m_utilityManager = nullptr;

            delete m_nameManager;

            m_nameManager = nullptr;

            delete m_entityManager;
            
            m_entityManager = nullptr;
        }

        EntityHandle World::CreateEntity(const std::string &name)
        {
            auto entity = m_entityManager->Create( );

            entity->SetName( name );

            return entity;
        }

        EntityHandle World::CreateEntityFromArchetype(
            const std::string &fileName, 
            const std::string &name
        )
        {
            auto cache = m_archetypeCache.find( fileName );

            EntityHandle entity;

            if (cache == m_archetypeCache.end( ))
            {
                std::string jsonData;

                UAssert(
                    fs::LoadAllText( fileName, jsonData ),
                    "Failed to load archetype.\nfile: %s", 
                    fileName.c_str( )
                );

                std::string jsonError;

                auto data = Json::parse( jsonData, jsonError );

                UAssert(
                    jsonError.empty( ),
                    "Failed to load archetype.\nJSON error: %s\nfile: %s",
                    jsonError.c_str( ),
                    fileName.c_str( )
                );

                m_archetypeCache[ fileName ] = data;

                entity = loadArchetype( data );
            }
            else
            {
                entity = loadArchetype( cache->second );
            }
            
            if (entity)
                entity->SetName( name );

            return entity;
        }

        EntityHandle World::CreateEntityFromArchetype(const resources::ResourceReference &resource)
        {
            UAssert( m_owner != nullptr,
                "Cannot create archetype without scene."    
            );

            auto *archetype = resource.Load<resources::ArchetypeData>( 
                m_owner->GetResourceManager( ) 
            );

            // if we couldn't load the archetype, return an invalid entity handle
            return archetype ? archetype->Instantiate( this ) : EntityHandle::Invalid( );
        }

        EntityHandle World::GetEntity(EntityID id) const
        {
            return m_entityManager->GetEntityByID( id );
        }

        EntityHandle World::GetEntityFromName(const std::string &name) const
        {
            return m_nameManager->GetEntity( name );
        }

        EntityHandleVector World::GetRootEntities(void) const
        {
            return m_entityManager->GetRootEntities( );
        }

        EntityHandleVector World::GetActiveEntities(void) const
        {
            return m_entityManager->GetActiveEntities( );
        }

        EntityHandleVector World::GetEntitiesFromName(const std::string &name) const
        {
            return m_nameManager->GetEntities( name );
        }

        EntityHandleVector World::GetEntitiesFromFilter(const Filter &filter) const
        {
            return m_entityManager->GetEntities( filter );
        }

        void World::Update(void)
        {
            clearDeletionQueue( );

            Dispatch( WORLD_UPDATE, EventArgs::Empty );
        }

        void World::EditorUpdate(void)
        {
            clearDeletionQueue( );

            Dispatch( WORLD_EDITOR_UPDATE, EventArgs::Empty );
        }

        void World::Render(void)
        {
            Dispatch( WORLD_RENDER, EventArgs::Empty );
        }

        void World::EditorRender(void)
        {
            Dispatch( WORLD_EDITOR_RENDER, EventArgs::Empty );
        }

        const EntityHandle &World::GetSettings(void) const
        {
            return m_settings;
        }

        EntityManager *World::GetEntityManager(void) const
        {
            return m_entityManager;
        }

        SystemManager *World::GetSystemManager(void) const
        {
            return m_systemManager;
        }

        Scene *World::GetOwner(void) const
        {
            return m_owner;
        }

        void World::setOwner(Scene *owner)
        {
            UAssert( m_owner == nullptr,
                "World owner already set."  
            );

            m_owner = owner;

            // make sure we have a the world configuration component
            if (!m_settings->HasComponent<WorldConfig>( ) )
                m_settings->AddComponent<WorldConfig>( );

            m_entityManager->initializeScene( );
            m_systemManager->initializeScene( );
        }

        void World::queueEntityDeletion(Entity *entity)
        {
            m_deleted.emplace_back( EntityHandle( entity ) );
        }

        void World::MergeWorld(
            resources::ResourceManager &resourceManager,
            resources::ResourceReference &worldResource
        )
        {
            auto data = worldResource.Load<resources::WorldData>( resourceManager );

            UAssert( data != nullptr,
                "Unable to load reference of world to merge."
            );

            WorldSerializer::MergeDeserialize( data->GetData( ), this );
        }

        void World::deleteEntity(const EntityHandle &entity)
        {
            m_nameManager->Remove( entity->GetID( ) );
            m_entityManager->Remove( entity );
        }
                
        void World::clearDeletionQueue(void)
        {
            std::lock_guard<std::mutex> lock( m_deletionMutex );

            for (auto &entity : m_deleted)
                m_entityManager->BeforeRemove( entity.Get( ) );

            while (m_deleted.size( ))
            {
                auto &entity = m_deleted.back( );

                m_deleted.pop_back( );

                deleteEntity( entity );
            }
        }

        EntityHandle World::loadArchetype(const Json &data)
        {
            try
            {
                return EntitySerializer( ).DeserializeArchetype( this, data );
            }
            catch (SerializationException &e)
            {
                UError( 
                    "Unable to deserialize archetype.\nerror: %s",
                    e.GetError( ).c_str( )
                );

                return EntityHandle::Invalid( );
            }
        }
    }
}