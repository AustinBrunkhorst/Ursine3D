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

#include "WorldConfig.h"

#include "Entity.h"

namespace ursine
{
    namespace ecs
    {
        World::World(void)
            : _loaded(false)
            , _owner(nullptr)
        {

        }

        World::~World(void)
        {
            
        }

        void World::Load(const std::string &path)
        {
            UAssert(!_loaded, "World has already been loaded.");

            _loaded = true;
        }

        const Json &World::GetAttribute(const std::string &name) const
        {
            return _attributes[name];
        }

        void World::SetOwner(void *owner)
        {
            _owner = owner;
        }

        Entity *World::CreateEntity(void)
        {
            return nullptr;
        }

        Entity *World::CreateEntity(const std::string &identifier)
        {
            return nullptr;
        }

        Entity* World::CreateEntity(const std::string &identifier, const Json &merge)
        {
            return nullptr;
        }

        Entity *World::GetEntity(EntityID id) const
        {
            return nullptr;
        }

        Entity *World::GetEntity(const std::string &tag) const
        {
            return nullptr;
        }

        Entity *World::GetEntityUnique(EntityUniqueID unique_id) const
        {
            return nullptr;
        }

        const EntityVector &World::GetEntities(const std::string &group) const
        {
            return *new EntityVector( );
        }

        EntityVector World::GetEntities(const Filter &filter) const
        {
            return { };
        }

        void World::Update(void)
        {
            while (_deleted.size())
            {
                auto entity = _deleted.back();

                _deleted.pop_back( );
            }

        }

        template<>
        EntityManager *World::Manager(void)
        {
            return _entity_manager;
        }

        template<>
        SystemManager *World::Manager(void)
        {
            return _system_manager;
        }

        template<>
        TagManager *World::Manager(void)
        {
            return _tag_manager;
        }

        template<>
        GroupManager *World::Manager(void)
        {
            return _group_manager;
        }

        template<>
        UtilityManager *World::Manager(void)
        {
            return _utility_manager;
        }

        void World::deleteEntity(Entity *entity)
        {
            _deleted.push_back(entity);
        }
    }
}