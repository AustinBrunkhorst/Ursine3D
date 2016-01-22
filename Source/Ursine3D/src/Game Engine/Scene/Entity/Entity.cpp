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
            : m_active( true )
            , m_deleting( false )
            , m_deletionEnabled( true )
            , m_hierarchyChangeEnabled( true )
            , m_serializationEnabled( true )
            , m_visibleInEditor( true )
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
            // it's already being deleted or deletion is disabled
            if (!(m_active && !m_deleting) || !m_deletionEnabled)
                return;

			setDeletingTrue( );

            m_world->queueEntityDeletion( this );
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
            return m_deleting;
        }

        bool Entity::IsActive(void) const
        {
            return m_active;
        }

        bool Entity::IsAvailable(void) const
        {
            // active, but not deleting
            return m_active && !m_deleting;
        }

        bool Entity::IsDeletionEnabled(void) const
        {
            return m_deletionEnabled;
        }

        void Entity::EnableDeletion(bool enabled)
        {
            m_deletionEnabled = enabled;
        }

        bool Entity::IsHierarchyChangeEnabled(void) const
        {
            return m_hierarchyChangeEnabled;
        }

        void Entity::EnableHierarchyChange(bool enabled)
        {
            m_hierarchyChangeEnabled = enabled;
        }

        bool Entity::IsSerializationEnabled(void) const
        {
            return m_serializationEnabled;
        }

        void Entity::EnableSerialization(bool enabled)
        {
            m_serializationEnabled = enabled;
        }

        bool Entity::IsVisibleInEditor(void) const
        {
            return m_visibleInEditor;
        }

        void Entity::SetVisibleInEditor(bool visible)
        {
            m_visibleInEditor = visible;
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

        Entity *Entity::Clone(void)
        {
            return m_world->m_entityManager->Clone( this );
        }

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

        const std::vector<EntityID> *Entity::GetChildren(void) const
        {
            return m_world->m_entityManager->GetChildren( this );
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

        void Entity::setSystem(SystemTypeMask mask)
        {
            utils::FlagSet( m_systemMask, mask );
        }

        void Entity::unsetSystem(SystemTypeMask mask)
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
            m_active = true;
            m_deleting = false;

            // default settings
            m_deletionEnabled = true;
            m_hierarchyChangeEnabled = true;
            m_serializationEnabled = true;
            m_visibleInEditor = true;
        }

		void Entity::setDeletingTrue(void)
        {
			m_deleting = true;

			for (auto &child : GetTransform( )->GetChildren( ))
				child->GetOwner( )->setDeletingTrue( );
        }
    }
}

//To make gameplay pause: 