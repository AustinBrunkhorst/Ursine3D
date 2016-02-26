/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** WaypointComponent.h
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once
#include <unordered_map>

#include "Component.h"
#include "AIPriorityQueue.h"

namespace ursine
{
    namespace ecs
    {
        class WaypointAgent : public Component
        {
            NATIVE_COMPONENT;

            friend class WaypointSystem;

        public:
            WaypointAgent(void);

            Meta(Disable)
            Node *GetClosestNode(const Vec3 &pos);

            Meta(Disable)
            std::list<Node *> &GetClosedList(void);

            Meta(Disable)
            const std::vector<Node> &GetNodesList(void) const;

        private:
            Meta(Disable)
            std::vector<Node> m_nodes;

            Meta(Disable)
            std::list<Node *> m_closed;

        } Meta(Enable, DisplayName( "Waypoint Agent" ));

    }
}
