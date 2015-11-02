/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SystemManager.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "SystemConfig.h"
#include "EntitySystem.h"

#include "WorldManager.h"

#include <vector>

namespace ursine
{
    namespace ecs
    {
        class SystemManager final
            : public WorldManager
        {
        public:
            template<class SystemType>
            inline bool HasSystem(void);

            template<class SystemType>
            inline SystemType *GetSystem(void);

        private:
            friend class World;

            std::vector<EntitySystem*> m_systems;

            SystemManager(World *world);
            ~SystemManager(void);

            void OnInitialize(void) override;

            void onAfterLoad(void);
        };
    }
}

#include "SystemManager.hpp"