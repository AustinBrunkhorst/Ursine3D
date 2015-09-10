/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EntitySerializer.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <unordered_map>

namespace ursine
{
    class Json;

    namespace ecs
    {
        class Entity;
        class EntityManager;

        class EntitySerializer final
        {
            typedef std::unordered_map<std::string, ComponentVector> ComponentCache;

            ComponentCache _cache;

            void deserialize(const Json &data,
                EntityManager *manager, Entity *entity, ComponentVector &created_out, bool no_check = false);

            void copyComponents(Entity *entity, EntityManager *manager, 
                const ComponentVector &components);

            std::string getIdentifier(const std::string &name, EntityManager *manager);
            Json loadIdentifier(const std::string &identifier);
        public:
            ~EntitySerializer(void);

            Json Serialize(Entity *entity) const;

            void Deserialize(const std::string &name, EntityManager *manager, 
                Entity *entity);

            void Deserialize(const std::string &name,
                const Json &merge, EntityManager *manager, Entity *entity);

            void ClearCache(void);
        };
    }
}
