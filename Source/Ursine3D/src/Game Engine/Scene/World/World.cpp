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

namespace ursine
{
    namespace ecs
    {
        World::World(void)
            : EventDispatcher( this )
            , m_entityManager( new EntityManager( this ) )
            , m_systemManager( new SystemManager( this ) )
            , m_nameManager( new NameManager( this ) )
            , m_utilityManager( new UtilityManager( this ) )
        {
            m_entityManager->OnInitialize( );
            m_systemManager->OnInitialize( );
            m_nameManager->OnInitialize( );
            m_utilityManager->OnInitialize( );
        }

        World::~World(void)
        {
            delete m_utilityManager;
            delete m_nameManager;
            delete m_systemManager;
            delete m_entityManager;
        }

        Entity *World::CreateEntity(void)
        {
            return m_entityManager->Create( );
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

        void World::deleteEntity(Entity *entity)
        {
            m_entityManager->BeforeRemove( entity );

            m_deleted.push_back( entity );
        }
    }
}