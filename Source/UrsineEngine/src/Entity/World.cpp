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

#include "EntityManager.h"
#include "SystemManager.h"
#include "TagManager.h"
#include "GroupManager.h"
#include "UtilityManager.h"

namespace ursine
{
    namespace ecs
    {
        World::World(void)
            : _loaded(false)
            , _owner(nullptr)
        {
            _camera.SetMoveSpeed({ 4.0f, 4.0f });
            _camera.SetZoomSpeed(2.0f);
            _camera.SetShakeSpeed({ 5.5f, 5.5f });

            _camera.SetPositionConstraints(false);
            _camera.SetZoomConstraints(false);

            _entity_manager = addManager<EntityManager>();
            _system_manager = addManager<SystemManager>();
            _tag_manager = addManager<TagManager>();
            _group_manager = addManager<GroupManager>();
            _utility_manager = addManager<UtilityManager>();

            // initialize after all managers have been constructed
            for (auto manager : _manager_queue)
                manager->OnInitialize();
        }

        World::~World(void)
        {
            // delete in reverse order
            for (auto it = _manager_queue.rbegin(); it != _manager_queue.rend(); ++it)
                delete *it;
        }

        void World::Load(const std::string &path)
        {
            UAssert(!_loaded, "World has already been loaded.");

            _serializer.Deserialize(path + kWorldExtension, this);

            for (auto manager : _manager_queue)
                manager->OnWorldLoaded();

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
            return _entity_manager->Create();
        }

        Entity *World::CreateEntity(const std::string &identifier)
        {
            return _entity_manager->Create(identifier);
        }

        Entity* World::CreateEntity(const std::string &identifier, const Json &merge)
        {
            return _entity_manager->Create(identifier, merge);
        }

        Entity *World::GetEntity(EntityID id) const
        {
            return _entity_manager->GetEntity(id);
        }

        Entity *World::GetEntity(const std::string &tag) const
        {
            return _tag_manager->GetEntity(tag);
        }

        Entity *World::GetEntityUnique(EntityUniqueID unique_id) const
        {
            return _entity_manager->GetEntityUnique(unique_id);
        }

        const EntityVector &World::GetEntities(const std::string &group) const
        {
            return _group_manager->GetEntities(group);
        }

        EntityVector World::GetEntities(const Filter &filter) const
        {
            return _entity_manager->GetEntities(filter);
        }

        void World::Update(void)
        {
            while (_deleted.size())
            {
                auto entity = _deleted.back();

                _deleted.pop_back();

                _tag_manager->UnRegister(entity);
                _group_manager->Clear(entity);
                _utility_manager->Clear(entity);
                _entity_manager->Remove(entity);
            }

            _system_manager->Update();

            _camera.Update();
        }

        void World::Draw(void)
        {
            _system_manager->Draw();
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
            _entity_manager->BeforeRemove(entity);

            _deleted.push_back(entity);
        }
    }
}