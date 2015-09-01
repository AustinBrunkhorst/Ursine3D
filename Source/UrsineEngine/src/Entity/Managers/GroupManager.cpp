/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GroupManager.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "GroupManager.h"
#include "Entity.h"

namespace ursine
{
    namespace ecs
    {
        GroupManager::GroupManager(World *world)
            : WorldManager(world)
            , EventDispatcher(this) { }

        GroupManager::~GroupManager(void) { }

        void GroupManager::Add(const std::string &group, Entity *entity)
        {
            auto &groups = _id_map[entity->_unique_id];

            // already have this group
            if (find(groups.begin(), groups.end(), group) != groups.end())
                return;

            GroupEventArgs e(entity, group);

            Dispatch(GM_GROUP_ADDED, &e);

            groups.push_back(group);

            _groups[group].push_back(entity);
        }

        void GroupManager::Remove(const std::string &group, Entity *entity)
        {
            auto &entities = _groups[group];

            auto search = find(entities.begin(), entities.end(), entity);

            // doesn't have this group
            if (search == entities.end())
                return;

            GroupEventArgs e(entity, group);

            Dispatch(GM_GROUP_REMOVED, &e);

            entities.erase(search);

            auto &groups = _id_map[entity->_unique_id];

            groups.erase(find(groups.begin(), groups.end(), group));
        }

        void GroupManager::Clear(Entity *entity)
        {
            auto &groups = _id_map[entity->_unique_id];

            while (!groups.empty())
                Remove(groups.front(), entity);
        }

        const EntityVector &GroupManager::GetEntities(const std::string &group)
        {
            return _groups[group];
        }

        const EntityGroupVector &GroupManager::GetGroups(const Entity *entity)
        {
            return _id_map[entity->_unique_id];
        }

        bool GroupManager::HasGroup(const std::string &group, const Entity *entity)
        {
            auto &groups = _id_map[entity->_unique_id];

            auto search = std::find(groups.begin(), groups.end(), group);

            return search != groups.end();
        }
    }
}
