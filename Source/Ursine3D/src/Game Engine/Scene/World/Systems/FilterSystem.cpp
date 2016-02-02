/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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

            auto *entity = args->entity;

            auto interests = m_filter.Matches( entity );
            auto removed = args->type == WORLD_ENTITY_COMPONENT_REMOVED;
				
            if (removed || !interests)
            {
                Remove( entity );
            }
            else if(interests)
            {
                Add( entity );
            }
        }

        void FilterSystem::onEntityRemoved(EVENT_HANDLER(World))
        {
            EVENT_ATTRS(World, EntityEventArgs);

            auto entity = args->entity;

            // remove the entity from this system if it interested us before
            // removal
            if (m_filter.Matches( entity ))
                Remove( entity );
        }

        void FilterSystem::onUpdate(EVENT_HANDLER(World))
        {
            Begin( );

            for (auto &entity : m_active)
				Process( entity.second );

            End( );
        }

        void FilterSystem::Add(Entity *entity)
        {
            if (entity->IsActive( ))
            { 
                entity->setSystem( GetTypeMask( ) );

                Enable( entity );
            }
        }

        void FilterSystem::Remove(Entity *entity)
        {
            entity->unsetSystem( GetTypeMask( ) );

            Disable( entity );
        }

        void FilterSystem::Enable(Entity *entity)
        {
            auto uniqueID = entity->GetUniqueID( );

            // already enabled
            if (m_active.find( uniqueID ) != m_active.end( ))
                return;

            m_active[ uniqueID ] = entity;
        }

        void FilterSystem::Disable(Entity *entity)
        {
            auto uniqueID = entity->GetUniqueID( );

            auto search = m_active.find( uniqueID );

            // doesn't exist
            if (search == m_active.end( ))
                return;

            m_active.erase( search );
        }

        void FilterSystem::OnInitialize(void)
        {
            m_world->Listener( this )
                .On( WORLD_ENTITY_COMPONENT_ADDED, &FilterSystem::onComponentChange )
                .On( WORLD_ENTITY_COMPONENT_REMOVED, &FilterSystem::onComponentChange )
                .On( WORLD_ENTITY_REMOVED, &FilterSystem::onEntityRemoved )
                .On( m_updateType, &FilterSystem::onUpdate, m_updatePriority );

            Initialize( );
        }

        void FilterSystem::Initialize(void)
        {
            
        }

        void FilterSystem::OnRemove(void)
        {
            m_world->Listener( this )
                .Off( WORLD_ENTITY_COMPONENT_ADDED, &FilterSystem::onComponentChange )
                .Off( WORLD_ENTITY_COMPONENT_REMOVED, &FilterSystem::onComponentChange )
                .Off( WORLD_ENTITY_REMOVED, &FilterSystem::onEntityRemoved )
                .Off( m_updateType, &FilterSystem::onUpdate );
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
