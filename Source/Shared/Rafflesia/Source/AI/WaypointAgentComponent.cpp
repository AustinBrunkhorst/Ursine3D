#include "Precompiled.h"

#include "WaypointAgentComponent.h"
#include "WaypointSystem.h"
#include<SystemManager.h>

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

        void WaypointAgent::OnInitialize()
        {
            auto entitySystem = GetOwner()->GetWorld()->GetEntitySystem<WaypointSystem>();

            if (!entitySystem)
                return;


            auto map = entitySystem->GetWaypointMap();

            if (map.empty())
                return;

            for (auto wpPair : map)
            {
                m_nodes.push_back(Node(wpPair.second, 0,0));
            }
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

        SVec3 WaypointAgent::GetTargetPathPosition() const
        {
            // if there are no waypoints on the list, just return target's position
            if (m_closed.empty() == true)
            {
                return m_target;
            }

            return m_closed.begin()._Ptr->_Myval->waypoint.Get()->GetOwner()->GetTransform()->GetWorldPosition();
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

        void WaypointAgent::UpdateClosed()
        {
            if (m_closed.empty() == true)
                return;

            Waypoint *topWp = m_closed.begin()._Ptr->_Myval->waypoint.Get();

            float arrivedDist = topWp->GetRadius();

            if (GetOwner()->GetTransform()->GetWorldPosition().Distance(topWp->GetPosition()) <= arrivedDist)
            {
                m_closed.pop_front();
            }
        }
    }
}
