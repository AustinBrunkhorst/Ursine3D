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

        std::list<Node*>& WaypointAgent::GetClosedList(void)
        {
            return m_closed;
        }

        const std::vector<Node>& WaypointAgent::GetNodesList(void) const
        {
            return m_nodes;
        }
    }
}
