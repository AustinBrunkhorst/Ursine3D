/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** WaypointSystem.h
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "EntitySystem.h"
#include "AIMovementControllerComponent.h"

namespace ursine
{
    namespace ecs
    {
        class AIControllerSystem
            : public EntitySystem
        {
            ENTITY_SYSTEM;

            // refering to the max size of the open list
            const int MAX_NODES = 100;

        public:
            AIControllerSystem(World *world);

        private:

            void OnInitialize(void) override;
            void OnRemove(void) override;

            void onComponentAdded(EVENT_HANDLER(World));
            void onComponentRemoved(EVENT_HANDLER(World));

            void onUpdate(EVENT_HANDLER(World));

            std::vector<Component::Handle<AIMovementController> > m_kinematicControllers;
        } Meta(Enable, AutoAddEntitySystem);
    }
}
