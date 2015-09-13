/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Entity.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/


#include "UrsinePrecompiled.h"

#include "Entity.h"

#include "LocalTimerManager.h"
#include "LocalTweenManager.h"

namespace ursine
{
    namespace ecs
    {
        ////////////////////////////////////////////////////////////////////////
        // Contructors
        ////////////////////////////////////////////////////////////////////////

        Entity::Entity(World *world, EntityUniqueID id)
            : m_flags( ACTIVE )
            , m_id( id )
            , m_world( world )
            , m_systemMask( 0 )
            , m_typeMask( 0 )
        {
        }

        ////////////////////////////////////////////////////////////////////////
        // State/Identification
        ////////////////////////////////////////////////////////////////////////

        void Entity::Delete(void)
        {
            // it's already being deleted
            if (m_flags != ACTIVE)
                return;

            utils::FlagSet( m_flags, DELETING );

            m_world->deleteEntity( this );
        }

        EntityID Entity::GetID(void) const
        {
            return m_id;
        }

        EntityUniqueID Entity::GetUniqueID(void) const
        {
            return m_uniqueID;
        }

        bool Entity::IsDeleting(void) const
        {
            return utils::IsFlagSet( m_flags, DELETING );
        }

        bool Entity::IsActive(void) const
        {
            return utils::IsFlagSet( m_flags, ACTIVE );
        }

        bool Entity::IsAvailable(void) const
        {
            // active, but not deleting
            return (m_flags & (ACTIVE | DELETING)) == ACTIVE;
        }

        World *Entity::GetWorld(void) const
        {
            return m_world;
        }

        Transform *Entity::GetTransform(void) const
        {
            return m_transform;
        }

        ////////////////////////////////////////////////////////////////////////
        // Tags
        ////////////////////////////////////////////////////////////////////////

        const std::string &Entity::GetTag(void) const
        {
            return *new std::string( );
            //return m_world->Manager<TagManager>()->GetTag(this);
        }

        void Entity::SetTag(const std::string &tag)
        {
            //m_world->Manager<TagManager>()->Register(tag, this);
        }

        ////////////////////////////////////////////////////////////////////////
        // Groups
        ////////////////////////////////////////////////////////////////////////

        const EntityGroupVector &Entity::GetGroups(void) const
        {
            return *new EntityGroupVector( );
            //return m_world->Manager<GroupManager>()->GetGroups(this);
        }

        void Entity::AddGroup(const std::string &group)
        {
            //m_world->Manager<GroupManager>()->Add(group, this);
        }

        void Entity::RemoveGroup(const std::string &group)
        {
            //m_world->Manager<GroupManager>()->Remove(group, this);
        }

        bool Entity::HasGroup(const std::string &group) const
        {
            return false;
            //return m_world->Manager<GroupManager>()->HasGroup(group, this);
        }

        ////////////////////////////////////////////////////////////////////////
        // Utilities
        ////////////////////////////////////////////////////////////////////////

        LocalTimerManager &Entity::GetTimers(void)
        {
            return *new LocalTimerManager( nullptr );
            //return m_world->Manager<UtilityManager>()->GetTimers( this );
        }

        LocalTweenManager &Entity::GetTweens(void)
        {
            return *new LocalTweenManager( nullptr );
            //return m_world->Manager<UtilityManager>()->GetTweens( this );
        }

        ////////////////////////////////////////////////////////////////////////
        // Components
        ////////////////////////////////////////////////////////////////////////

        bool Entity::HasComponent(ComponentTypeMask mask) const
        {
            return utils::IsFlagSet( m_typeMask, mask );
        }

        Component *Entity::GetComponent(ComponentTypeID id) const
        {
            return nullptr;
            //return m_world->Manager<EntityManager>()->GetComponent(this, id);
        }

        ComponentVector Entity::GetComponents(void) const
        {
            return { };
            //return m_world->Manager<EntityManager>()->GetComponents(this);
        }

        void Entity::RemoveComponent(ComponentTypeID id)
        {
            //m_world->Manager<EntityManager>()->RemoveComponent(this, id);
        }

        ////////////////////////////////////////////////////////////////////////
        // Events
        ////////////////////////////////////////////////////////////////////////

        void Entity::Dispatch(EventID event, const EventArgs *args)
        {
            //m_world->Manager<EntityManager>()->GetEntityEvents(this).Dispatch(event, this, args);
        }

        ////////////////////////////////////////////////////////////////////////
        // Private Functions
        ////////////////////////////////////////////////////////////////////////

        void Entity::setSystem(ComponentTypeMask mask)
        {
            utils::FlagSet( m_systemMask, mask );
        }

        void Entity::unsetSystem(ComponentTypeMask mask)
        {
            utils::FlagUnset( m_systemMask, mask );
        }

        void Entity::setType(ComponentTypeMask mask)
        {
            utils::FlagSet( m_typeMask, mask );
        }

        void Entity::unsetType(ComponentTypeMask mask)
        {
            utils::FlagUnset( m_typeMask, mask );
        }

        void Entity::reset(void)
        {
            m_systemMask = 0;
            m_typeMask = 0;

            // active and not deleting
            m_flags = ACTIVE;
        }
    }
}
