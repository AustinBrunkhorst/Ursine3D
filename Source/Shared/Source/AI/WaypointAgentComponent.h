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

            void SetTarget(const SVec3 &pos);

            const SVec3 &GetTarget(void) const;


            EditorField(
                float UpdateTimer,
                GetUpdateTimer,
                SetUpdateTimer
                );

            float GetUpdateTimer(void) const;

            void SetUpdateTimer(float time);

            //Meta(Disable)
            //SVec3 &GetClosestNodePos(const Vec3 &pos);
            //
            //Meta(Disable)
            //SVec3 &GetClosestNodeToTarget(const SVec3 &pos, const SVec3 &target);

            Meta(Disable)
            std::list<Node *> &GetClosedList(void);

            // add function that is going to get the closest node between us and the target

            Meta(Disable)
            const std::vector<Node> &GetNodesList(void) const;

            void ResetTimer(void);

            void UpdateTimer(float dt);

            bool CanUpdate(void) const;

        private:
            std::vector<Node> m_nodes;
            std::list<Node *> m_closed;

            SVec3 m_target;

            // value that the timer is always reset to
            float m_timerLength;

            // timer that is checked when we want to see if we should update
            float m_timer;
        } Meta(Enable, DisplayName( "Waypoint Agent" ));

    }
}
