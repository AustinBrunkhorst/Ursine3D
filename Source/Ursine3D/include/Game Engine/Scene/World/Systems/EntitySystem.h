/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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

namespace ursine
{
    namespace ecs
    {
        // forward declarations
        class World;

        class EntitySystem
        {
        public:
            EntitySystem(World *world);
            virtual ~EntitySystem(void) { }

            inline SystemTypeID GetTypeID(void) const;
            inline SystemTypeMask GetTypeMask(void) const;

        protected:
            World *m_world;

            LocalTimerManager m_timers;
            LocalTweenManager m_tweens;

            // Called when the system is initialized
            virtual void OnInitialize(void) { }

            // Called when a new scene is ready
            virtual void OnSceneReady(Scene *scene) { }

            // Called when the system is removed from the world
            virtual void OnRemove(void) { }

        private:
            // SystemManager is able to call OnInitialize and OnDestroy
            friend class SystemManager;

            SystemTypeID m_typeID;
        } Meta(Enable, WhiteListMethods);
    }
}

#include "EntitySystem.hpp"