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
        FilterSystem::FilterSystem(World *world, const Filter &filter, SystemPriority priority)
            : EntitySystem( world, priority )
              , m_filter( filter ) { }

        void FilterSystem::onComponentChange(EVENT_HANDLER(EntityManager))
        {
            EVENT_ATTRS(EntityManager, EntityEventArgs);

            auto entity = args->entity;

            auto contains = utils::IsFlagSet( GetTypeMask( ), entity->m_systemMask );
            auto interests = m_filter.Matches( entity );
            auto removed = args->event_type == EM_COMPONENT_REMOVED;

            if (interests && !contains && !removed)
            {
                Add( entity );
            }
            else if (!interests && contains)
            {
                Remove( entity );
            }
            else if (entity->IsAvailable( ))
            {
                Enable( entity );
            }
            else
            {
                Disable( entity );
            }
        }

        void FilterSystem::onEntityRemoved(EVENT_HANDLER(EntityManager))
        {
            EVENT_ATTRS(EntityManager, EntityEventArgs);

            auto entity = args->entity;

            // remove the entity from this system if it interested us before
            // removal
            if (m_filter.Matches( entity ))
                Remove( entity );
        }

        void FilterSystem::onUpdate(EVENT_HANDLER(SystemManager))
        {
            Begin( );

            for (auto entity : m_active)
                Process( entity.second );

            End( );
        }

        void FilterSystem::Add(Entity *entity)
        {
            entity->setSystem( GetTypeMask( ) );

            if (entity->IsActive( ))
                Enable( entity );
        }

        void FilterSystem::Remove(Entity *entity)
        {
            entity->unsetSystem( GetTypeMask( ) );

            Disable( entity );
        }

        void FilterSystem::Enable(Entity *entity)
        {
            auto id = entity->GetID( );

            URSINE_TODO( "Optimize with binary search" )
            ;

            // already enabled
            if (m_active.find( id ) != m_active.end( ))
                return;

            m_active[ id ] = entity;
        }

        void FilterSystem::Disable(Entity *entity)
        {
            auto id = entity->GetID( );

            // doesn't exist
            if (m_active.find( id ) == m_active.end( ))
                return;

            m_active.erase( id );
        }

        void FilterSystem::OnInitialize(void)
        {
            m_world->GetEntityManager( )->Listener( this )
                .On( EM_COMPONENT_ADDED, &FilterSystem::onComponentChange )
                .On( EM_COMPONENT_REMOVED, &FilterSystem::onComponentChange )
                .On( EM_ENTITY_REMOVED, &FilterSystem::onEntityRemoved );

            m_world->GetSystemManager( )->Listener( this )
                .On( SM_UPDATE, &FilterSystem::onUpdate );
        }

        void FilterSystem::OnRemove(void)
        {
            m_world->GetEntityManager( )->Listener( this )
                .Off( EM_COMPONENT_ADDED, &FilterSystem::onComponentChange )
                .Off( EM_COMPONENT_REMOVED, &FilterSystem::onComponentChange )
                .Off( EM_ENTITY_REMOVED, &FilterSystem::onEntityRemoved );

            m_world->GetSystemManager( )->Listener( this )
                .Off( SM_UPDATE, &FilterSystem::onUpdate );
        }
    }
}
