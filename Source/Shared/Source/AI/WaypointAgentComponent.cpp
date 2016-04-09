#include "Precompiled.h"

#include "WaypointAgentComponent.h"

#include <limits>
#include <list>
namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( WaypointAgent );

        WaypointAgent::WaypointAgent(): BaseComponent()
            , m_target(0,0,0)
            , m_timerLength(0.5f)
            , m_timer(0.0f)
        {
        }

        Node* WaypointAgent::GetClosestNode(const Vec3& pos)
        {
            //float shortest_dist = Mathf.Infinity;
            float shortestDistance = std::numeric_limits<float>::max();
            
            //Node closest_node = null;
            Node *closestNode = nullptr;
            
            //foreach(var node in m_nodes)
            for (auto &node : m_nodes)
            {
            //    float dist = (pos - node.waypoint.position).magnitude;
                float dist = pos.Distance(node.waypoint->GetPosition());
            
            //    if (dist < shortest_dist)
                if (dist < shortestDistance)
                {
                    shortestDistance = dist;
                    
                    closestNode = &node;
                    
                }
            }
            //return closest_node;
            return closestNode;
        }

        void WaypointAgent::SetTarget(const SVec3& pos)
        {
            m_target = pos;
        }

        const SVec3& WaypointAgent::GetTarget() const
        {
            return m_target;
        }

        float WaypointAgent::GetUpdateTimer() const
        {
            return m_timerLength;
        }

        void WaypointAgent::SetUpdateTimer(float time)
        {
            m_timerLength = time;
            m_timer = time;
        }

        std::list<Node*>& WaypointAgent::GetClosedList(void)
        {
            return m_closed;
        }

        const std::vector<Node>& WaypointAgent::GetNodesList(void) const
        {
            return m_nodes;
        }

        void WaypointAgent::ResetTimer(void)
        {
            m_timer = m_timerLength;
        }

        void WaypointAgent::UpdateTimer(float dt)
        {
            m_timer -= dt;
        }

        bool WaypointAgent::CanUpdate(void) const
        {
            return m_timer <= 0.0f;
        }
    }
}
