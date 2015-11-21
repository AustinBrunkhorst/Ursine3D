/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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
            , m_settings( nullptr )
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
            delete m_utilityManager;
            delete m_nameManager;
            delete m_entityManager;
            
            m_settings = nullptr;
        }

        Entity *World::CreateEntity(const std::string &name)
        {
            auto entity = m_entityManager->Create( );

            entity->SetName( name );

            return entity;
        }

        Entity *World::GetEntity(EntityID id) const
        {
            return m_entityManager->GetEntity( id );
        }

        Entity *World::GetEntityFromName(const std::string &name) const
        {
            return m_nameManager->GetEntity( name );
        }

        Entity *World::GetEntityUnique(EntityUniqueID uniqueID) const
        {
            return m_entityManager->GetEntityUnique( uniqueID );
        }

        EntityVector World::GetRootEntities(void) const
        {
            return m_entityManager->GetRootEntities( );
        }

        const EntityVector &World::GetActiveEntities(void) const
        {
            return m_entityManager->GetActiveEntities( );
        }

        const EntityVector &World::GetEntitiesFromName(const std::string &name) const
        {
            return m_nameManager->GetEntities( name );
        }

        EntityVector World::GetEntitiesFromFilter(const Filter &filter) const
        {
            return m_entityManager->GetEntities( filter );
        }

        void World::Update(void)
        {
            while (m_deleted.size( ))
            {
                auto entity = m_deleted.back( );

                m_deleted.pop_back( );

                m_nameManager->Remove( entity );
                m_entityManager->Remove( entity );
            }

            Dispatch( WORLD_UPDATE, EventArgs::Empty );
        }

        void World::Render(void)
        {
            Dispatch( WORLD_RENDER, EventArgs::Empty );
        }

        Entity *World::GetSettings(void) const
        {
            return m_settings;
        }

        SystemManager *World::GetSystemManager(void) const
        {
            return m_systemManager;
        }

        Screen *World::GetOwner(void) const
        {
            return m_owner;
        }

        void World::SetOwner(Screen *owner)
        {
            m_owner = owner;
        }

        void World::dispatchLoad(void)
        {
            m_systemManager->onAfterLoad( );
        }

        void World::deleteEntity(Entity *entity)
        {
            m_entityManager->BeforeRemove( entity );

            m_deleted.push_back( entity );
        }
    }
}