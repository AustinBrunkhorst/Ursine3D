/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TagManager.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "TagManager.h"
#include "EntityManager.h"

namespace Ursine
{
    namespace ECS
    {
        TagManager::TagManager(World *world)
            : WorldManager(world)
            , EventDispatcher(this)
        {
            
        }

        TagManager::~TagManager(void)
        {
            
        }

        const std::string &TagManager::GetTag(const Entity *entity)
        {
            return _id_map[entity->_unique_id];
        }

        Entity* TagManager::GetEntity(const std::string &tag) const
        {
            auto entity = _tags.find(tag);

            return entity == _tags.end() ? nullptr : entity->second;
        }

        void TagManager::Register(const std::string &tag, Entity *entity)
        {
        #ifdef CONFIG_DEBUG
            UAssert(_tags.find(tag) == _tags.end(), "Tag already exists.");
        #endif

            _tags[tag] = entity;
            _id_map[entity->_unique_id] = tag;

            TagEventArgs e = { tag, entity };

            Dispatch(TM_REGISTERED, &e);
        }

        void TagManager::UnRegister(Entity *entity)
        {
            auto tag = _id_map.find(entity->_unique_id);

            if (tag == _id_map.end())
                return;

            TagEventArgs e = { tag->second, entity };

            Dispatch(TM_UNREGISTERED, &e);

            _tags.erase(tag->second);
            _id_map.erase(tag);
        }
    }
}