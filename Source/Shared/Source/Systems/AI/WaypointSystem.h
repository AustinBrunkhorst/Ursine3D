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
#include "WaypointComponent.h"
#include "WaypointAgentComponent.h"

namespace ursine
{
    namespace ecs
    {
        class WaypointSystem 
            : public EntitySystem
        {
            ENTITY_SYSTEM;

            // refering to the max size of the open list
            const int MAX_NODES = 100;

        public:
            WaypointSystem(World *world);

            Meta(Disable)
            void CalculatePath(WaypointAgent &agent, const Vec3 &goal);

        private:

            void OnInitialize(void) override;
            void OnRemove(void) override;

            void onComponentAdded(EVENT_HANDLER(World));
            void onComponentRemoved(EVENT_HANDLER(World));

            static float CalculateHeurisitic(const Vec3 &pos, const Vec3 &goal);

            Meta(Disable)
            std::unordered_map<EntityUniqueID, Component::Handle<Waypoint> > m_waypoints;

        } Meta(Enable);
    }
}
