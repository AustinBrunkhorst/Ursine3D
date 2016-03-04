/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EntityHandle.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "EntityConfig.h"

namespace ursine
{
    namespace ecs
    {
        class EntityManager;
        class Entity;

        class EntityHandle
        {
        public:
            EntityHandle(void);
            EntityHandle(Entity *entity);
            EntityHandle(const EntityHandle &rhs);
            EntityHandle(const EntityHandle &&rhs);
            EntityHandle &operator=(const EntityHandle &rhs);
            EntityHandle &operator=(const EntityHandle &&rhs);

            explicit operator bool(void) const;
            bool operator==(const EntityHandle &rhs) const;
            bool operator!=(const EntityHandle &rhs) const;
            Entity *operator->(void) const;

            static const EntityHandle &Invalid(void);

            bool IsValid(void) const;

            Entity *Get(void) const;

        private:
            friend class Entity;
            friend class World;
            friend class EntityManager;
            friend struct std::hash<EntityHandle>;

            EntityID m_id;
            EntityIDVersion m_version;
            const EntityManager *m_manager;
        };
    }
}

#include "EntityHandle.hpp"