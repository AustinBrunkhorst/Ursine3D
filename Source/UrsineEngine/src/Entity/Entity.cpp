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
#include "EntityManager.h"
#include "TagManager.h"
#include "GroupManager.h"
#include "UtilityManager.h"

namespace Ursine
{
    namespace ECS
    {
        ////////////////////////////////////////////////////////////////////////
        // Contructors
        ////////////////////////////////////////////////////////////////////////
        
        Entity::Entity(World *world, EntityUniqueID id) 
            : _flags(ACTIVE)
            , _id(id)
            , _world(world)
            , _system_mask(0)
            , _type_mask(0)
        {
            
        }

        ////////////////////////////////////////////////////////////////////////
        // State/Identification
        ////////////////////////////////////////////////////////////////////////

        void Entity::Delete(void)
        {
            // it's already being deleted
            if (_flags != ACTIVE)
                return;

            Utils::FlagSet(_flags, DELETING);

            _world->deleteEntity(this);
        }

        EntityID Entity::GetID(void) const
        {
            return _id;
        }

        EntityUniqueID Entity::GetUniqueID(void) const
        {
            return _unique_id;
        }

        bool Entity::IsDeleting(void) const
        {
            return Utils::IsFlagSet(_flags, DELETING);
        }

        bool Entity::IsActive(void) const
        {
            return Utils::IsFlagSet(_flags, ACTIVE);
        }

        bool Entity::IsAvailable(void) const
        {
            // active, but not deleting
            return (_flags & (ACTIVE | DELETING)) == ACTIVE;
        }

        World *Entity::GetWorld(void) const
        {
            return _world;
        }

        Transform *Entity::GetTransform(void) const
        {
            return _transform;
        }

        ////////////////////////////////////////////////////////////////////////
        // Tags
        ////////////////////////////////////////////////////////////////////////

        const std::string &Entity::GetTag(void) const
        {
            return _world->Manager<TagManager>()->GetTag(this);
        }

        void Entity::SetTag(const std::string &tag)
        {
            _world->Manager<TagManager>()->Register(tag, this);
        }

        ////////////////////////////////////////////////////////////////////////
        // Groups
        ////////////////////////////////////////////////////////////////////////

        const EntityGroupVector &Entity::GetGroups(void) const
        {
            return _world->Manager<GroupManager>()->GetGroups(this);
        }

        void Entity::AddGroup(const std::string &group)
        {
            _world->Manager<GroupManager>()->Add(group, this);
        }

        void Entity::RemoveGroup(const std::string &group)
        {
            _world->Manager<GroupManager>()->Remove(group, this);
        }

        bool Entity::HasGroup(const std::string& group) const
        {
            return _world->Manager<GroupManager>()->HasGroup(group, this);
        }

        ////////////////////////////////////////////////////////////////////////
        // Utilities
        ////////////////////////////////////////////////////////////////////////

        LocalTimerManager &Entity::GetTimers(void)
        {
            return _world->Manager<UtilityManager>()->GetTimers( this );
        }

        LocalTweenManager &Entity::GetTweens(void)
        {
            return _world->Manager<UtilityManager>()->GetTweens( this );
        }

        ////////////////////////////////////////////////////////////////////////
        // Components
        ////////////////////////////////////////////////////////////////////////

        bool Entity::HasComponent(ComponentTypeMask mask) const
        {
            return Utils::IsFlagSet(_type_mask, mask);
        }

        Component *Entity::GetComponent(ComponentTypeID id) const
        {
            return _world->Manager<EntityManager>()->GetComponent(this, id);
        }

        ComponentVector Entity::GetComponents(void) const
        {
            return _world->Manager<EntityManager>()->GetComponents(this);
        }

        void Entity::RemoveComponent(ComponentTypeID id)
        {
            _world->Manager<EntityManager>()->RemoveComponent(this, id);
        }

        ////////////////////////////////////////////////////////////////////////
        // Events
        ////////////////////////////////////////////////////////////////////////

        void Entity::Dispatch(EventID event, const EventArgs *args)
        {
            _world->Manager<EntityManager>()->GetEntityEvents(this).Dispatch(event, this, args);
        }

        ////////////////////////////////////////////////////////////////////////
        // Private Functions
        ////////////////////////////////////////////////////////////////////////
        
        void Entity::setSystem(ComponentTypeMask mask)
        {
            Utils::FlagSet(_system_mask, mask);
        }

        void Entity::unsetSystem(ComponentTypeMask mask)
        {
            Utils::FlagUnset(_system_mask, mask);
        }

        void Entity::setType(ComponentTypeMask mask)
        {
            Utils::FlagSet(_type_mask, mask);
        }

        void Entity::unsetType(ComponentTypeMask mask)
        {
            Utils::FlagUnset(_type_mask, mask);
        }

        void Entity::reset(void)
        {
            _system_mask = 0;
            _type_mask = 0;

            // active and not deleting
            _flags = ACTIVE;
        }
    }
}