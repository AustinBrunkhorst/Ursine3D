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

#include "EntityManager.h"
#include "NameManager.h"
#include "UtilityManager.h"

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
              , m_uniqueID( 0 )
              , m_world( world )
              , m_transform( nullptr )
              , m_systemMask( 0 )
              , m_typeMask( 0 ) { }

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
        // Naming
        ////////////////////////////////////////////////////////////////////////

        const std::string &Entity::GetName(void) const
        {
            return m_world->m_nameManager->GetName( this );
        }

        void Entity::SetName(const std::string &name)
        {
            m_world->m_nameManager->SetName( this, name );
        }

        ////////////////////////////////////////////////////////////////////////
        // Utilities
        ////////////////////////////////////////////////////////////////////////

        LocalTimerManager &Entity::GetTimers(void)
        {
            return m_world->m_utilityManager->GetTimers( this );
        }

        LocalTweenManager &Entity::GetTweens(void)
        {
            return m_world->m_utilityManager->GetTweens( this );
        }

        ////////////////////////////////////////////////////////////////////////
        // Components
        ////////////////////////////////////////////////////////////////////////

        void Entity::AddComponent(Component *component)
        {
            m_world->m_entityManager->AddComponent( this, component );
        }

        bool Entity::HasComponent(ComponentTypeMask mask) const
        {
            return utils::IsFlagSet( m_typeMask, mask );
        }

        Component *Entity::GetComponent(ComponentTypeID id) const
        {
            return m_world->m_entityManager->GetComponent( this, id );
        }

        ComponentVector Entity::GetComponents(void) const
        {
            return m_world->m_entityManager->GetComponents( this );
        }

	    Component* Entity::GetComponentInChildren(ComponentTypeID id) const
	    {
			return m_world->m_entityManager->GetComponentInChildren( this, id );
	    }

	    Component* Entity::GetComponentInParent(ComponentTypeID id) const
	    {
			return m_world->m_entityManager->GetComponentInParent( this, id );
	    }

	    ComponentVector Entity::GetComponentsInChildren(ComponentTypeID id) const
	    {
			return m_world->m_entityManager->GetComponentsInChildren( this, id );
	    }

	    ComponentVector Entity::GetComponentsInParents(ComponentTypeID id) const
	    {
			return m_world->m_entityManager->GetComponentsInParents( this, id );
	    }

        uint Entity::GetSiblingIndex(void) const
        {
            return m_world->m_entityManager->GetSiblingIndex( this );
        }

        void Entity::SetAsFirstSibling(void)
        {
            m_world->m_entityManager->SetAsFirstSibling( this );
        }

        void Entity::SetSiblingIndex(uint index) const
        {
            m_world->m_entityManager->SetSiblingIndex( this, index );
        }

        void Entity::RemoveComponent(ComponentTypeID id)
        {
            m_world->m_entityManager->RemoveComponent( this, id );
        }

        ////////////////////////////////////////////////////////////////////////
        // Events
        ////////////////////////////////////////////////////////////////////////

        void Entity::Dispatch(EventID event, const EventArgs *args)
        {
            m_world->m_entityManager->GetEntityEvents( this ).Dispatch( event, this, args );
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
