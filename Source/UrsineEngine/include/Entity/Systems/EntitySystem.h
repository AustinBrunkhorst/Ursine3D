/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EntitySystem.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "SystemConfig.h"

#include "World.h"

#include "LocalTimerManager.h"
#include "LocalTweenManager.h"

namespace Ursine
{
    namespace ECS
    {
        // forward declarations
        class World;

        class EntitySystem
        {
            // SystemManager is able to call OnInitialize and OnDestroy
            friend class SystemManager;

            SystemTypeID _type_id;
            SystemTypeMask _type_mask;

            // update priority (higher updates first)
            int _priority;

            // compares systems based on priority
            static bool compare(const EntitySystem *a, const EntitySystem *b);

        protected:
            World *_world;

            Ursine::LocalTimerManager _timers;
            Ursine::LocalTweenManager _tweens;

            // Called when the system is initialized
            virtual void OnInitialize(void) { }

            // Called when the system is removed from the world
            virtual void OnRemove(void) { }

        public:
            EntitySystem(World *world, SystemPriority priority = 0);
            virtual ~EntitySystem() {}

            inline SystemTypeID GetTypeID(void);
            inline SystemTypeMask GetTypeMask(void);

            template<class SystemType>
            inline bool Is(void);
        };
    }
}

#include "EntitySystem.hpp"