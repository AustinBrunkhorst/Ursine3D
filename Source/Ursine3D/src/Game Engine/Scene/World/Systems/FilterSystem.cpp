/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** FilterSystem.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "FilterSystem.h"
#include "Entity.h"

namespace ursine
{
    namespace ecs
    {
        FilterSystem::FilterSystem(
            World *world, 
            const Filter &filter, 
            EventHandlerPriority updatePriority
        )
        : EntitySystem( world )
        , m_filter( filter )
        , m_updatePriority( updatePriority )
        , m_updateType( WORLD_UPDATE ) { }

        void FilterSystem::onComponentChange(EVENT_HANDLER(World))
        {
            EVENT_ATTRS(World, EntityEventArgs);

            auto &entity = args->entity;

            auto interests = m_filter.Matches( entity.Get( ) );
            auto removed = args->type == WORLD_ENTITY_COMPONENT_REMOVED;

            if (removed && !interests)
            {
                Remove( entity );
            }
            else if(!removed && interests)
            {
                Add( entity );
            }
        }

        void FilterSystem::onEntityRemoved(EVENT_HANDLER(World))
        {
            EVENT_ATTRS(World, EntityEventArgs);

            auto &entity = args->entity;

            // remove the entity from this system if it interested us before
            // removal
            if (m_filter.Matches( entity.Get( ) ))
                Remove( entity );
        }

        void FilterSystem::onUpdate(EVENT_HANDLER(World))
        {
            Begin( );

            for (auto &entity : m_active)
                Process( entity );

            End( );
        }

        void FilterSystem::Add(const EntityHandle &entity)
        {
            if (entity->IsActive( ))
            { 
                entity->setSystem( GetTypeMask( ) );

                Enable( entity );
            }
        }

        void FilterSystem::Remove(const EntityHandle &entity)
        {
            Disable( entity );

            entity->unsetSystem( GetTypeMask( ) );
        }

        void FilterSystem::Enable(const EntityHandle &entity)
        {
            m_active.insert( entity );
        }

        void FilterSystem::Disable(const EntityHandle &entity)
        {
            m_active.erase( entity );
        }

        void FilterSystem::Initialize(void)
        {

        }

        void FilterSystem::Remove(void)
        {
            
        }

        void FilterSystem::OnInitialize(void)
        {
            m_world->Listener( this )
                .On( WORLD_ENTITY_COMPONENT_ADDED, &FilterSystem::onComponentChange )
                .On( WORLD_ENTITY_COMPONENT_REMOVED, &FilterSystem::onComponentChange )
                .On( WORLD_ENTITY_REMOVED, &FilterSystem::onEntityRemoved )
                .On( m_updateType, &FilterSystem::onUpdate, m_updatePriority );

            auto entities = m_world->GetEntitiesFromFilter( m_filter );

            for (auto &entity : entities)
                Add( entity );

            Initialize( );
        }

        void FilterSystem::OnRemove(void)
        {
            m_world->Listener( this )
                .Off( WORLD_ENTITY_COMPONENT_ADDED, &FilterSystem::onComponentChange )
                .Off( WORLD_ENTITY_COMPONENT_REMOVED, &FilterSystem::onComponentChange )
                .Off( WORLD_ENTITY_REMOVED, &FilterSystem::onEntityRemoved )
                .Off( m_updateType, &FilterSystem::onUpdate );

            Remove( );
        }

        void FilterSystem::SetUpdateType(WorldEventType updateType)
        { 
            m_updateType = updateType;
        }

        WorldEventType FilterSystem::GetUpdateType(void) const
        {
            return m_updateType;
        }
    }
}
