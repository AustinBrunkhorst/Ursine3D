/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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

#if defined(URSINE_WITH_EDITOR)

#include "Notification.h"

#endif

namespace ursine
{
    namespace ecs
    {
        ////////////////////////////////////////////////////////////////////////
        // Contructors
        ////////////////////////////////////////////////////////////////////////

        Entity::Entity(const Entity &&rhs)
            : m_active( rhs.m_active )
            , m_deleting( rhs.m_deleting )
            , m_deletionEnabled( rhs.m_deletionEnabled )
            , m_hierarchyChangeEnabled( rhs.m_hierarchyChangeEnabled )
            , m_serializationEnabled( rhs.m_serializationEnabled )
            , m_visibleInEditor( rhs.m_visibleInEditor )
            , m_id( rhs.m_id )
            , m_version( rhs.m_version )
            , m_world( rhs.m_world )
            , m_transform( rhs.m_transform )
            , m_systemMask( rhs.m_systemMask )
            , m_typeMask( rhs.m_typeMask ) { }

        Entity::Entity(World *world, EntityID id)
            : m_active( true )
            , m_deleting( false )
            , m_deletionEnabled( true )
            , m_hierarchyChangeEnabled( true )
            , m_serializationEnabled( true )
            , m_visibleInEditor( true )
            , m_id( id )
            , m_version( 0 )
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
            if (!(m_active && !m_deleting))
                return;

            if (!m_deletionEnabled)
            {
            #if defined(URSINE_WITH_EDITOR)

                NotificationConfig error;

                error.type = NOTIFY_ERROR;
                error.header = "Error";
                error.message = "The entity <strong class=\"highlight\">"+ GetName( ) +"</strong> cannot be deleted.";

                EditorPostNotification( error );

            #endif
                return;
            }

            setDeletingTrue( );

            m_world->queueEntityDeletion( this );
        }

        EntityID Entity::GetID(void) const
        {
            return m_id;
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
            return m_world->m_nameManager->GetName( m_id );
        }

        void Entity::SetName(const std::string &name)
        {
            m_world->m_nameManager->SetName( m_id, name );
        }

        ////////////////////////////////////////////////////////////////////////
        // Utilities
        ////////////////////////////////////////////////////////////////////////

        EntityHandle Entity::Clone(void)
        {
            auto *entityManager = m_world->m_entityManager;

            return entityManager->Clone( this );
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

        void Entity::RemoveComponent(ComponentTypeID id)
        {
            m_world->m_entityManager->RemoveComponent( this, id );
        }

        bool Entity::HasComponent(const ComponentTypeMask &mask) const
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

        Component *Entity::GetComponentInChildren(ComponentTypeID id) const
        {
            return m_world->m_entityManager->GetComponentInChildren( this, id );
        }

        EntityHandle Entity::GetChildByName(const std::string &name) const
        {
            // get all children
            auto *children = m_world->m_entityManager->GetChildren( this );

            // search for desired child
            for (auto childID : *children)
            {
                auto entity = m_world->m_entityManager->GetEntityByID( childID );

                // check if names are same
                if (name == m_world->m_nameManager->GetName( childID ))
                    return entity;

                auto foundChild = entity->GetChildByName( name );

                if (foundChild)
                    return foundChild;
            }

            return EntityHandle::Invalid( );
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

        const EntityHandle &Entity::GetParent(void) const
        {
            auto parent = GetTransform( )->GetParent( );

            if (parent)
                return parent->GetOwner( );

            return EntityHandle::Invalid( );
        }

        const EntityHandle &Entity::GetRoot(void) const
        {
            return GetTransform( )->GetRoot( )->GetOwner( );
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

        void Entity::setType(const ComponentTypeMask &mask)
        {
            utils::FlagSet( m_typeMask, mask );
        }

        void Entity::unsetType(const ComponentTypeMask &mask)
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

            // resetting implies we're resusing it
            ++m_version;
        }

        void Entity::setDeletingTrue(void)
        {
            m_deleting = true;

            for (auto &child : GetTransform( )->GetChildren( ))
                child->GetOwner( )->setDeletingTrue( );
        }
    }
}
