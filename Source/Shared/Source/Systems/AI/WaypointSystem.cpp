#include "Precompiled.h"
#include "WaypointSystem.h"
#include "AIPriorityQueue.h"


namespace ursine
{
    namespace ecs
    {

        ENTITY_SYSTEM_DEFINITION( WaypointSystem );

        WaypointSystem::WaypointSystem(World *world)
            : EntitySystem( world )
        {
        }

        void WaypointSystem::CalculatePath(WaypointAgent &agent, const Vec3 &goal)
        {

            //var m_open = new Priority_Queue.HeapPriorityQueue<Node>(MAX_NODES);
            AIPriorityQueue openList;
            auto closedList = agent.GetClosedList();

            auto nodeList = agent.GetNodesList();

            // get closest waypoints
            auto start = agent.GetOwner()->GetTransform()->GetWorldPosition();
            Node *startData = agent.GetClosestNode(start);
            Node *goalData = agent.GetClosestNode(goal);
            
            if (startData == nullptr || goalData == nullptr)
            {
                //Debug.LogError("unable to find start node and/or goal node");
                //Debug.LogError("start node is set to: " + start_data);
                //Debug.LogError("end node is set to: " + goal_data);
            
                return;
            }

            // make it the first node on the open list (the start node)
            startData->hueristic = CalculateHeurisitic(startData->waypoint->GetPosition()
                                                      , goalData->waypoint->GetPosition());
            
            openList.Enqueue(startData);

            // while the open list is not empty
            while (!openList.IsEmpty())
            {
                // pop cheapest node off of the open list(parent node)
                auto pNode = openList.Dequeue();
                // if goal node, we found our path
                if (pNode == goalData)
                {
                    closedList.push_back(pNode);
                    //agent.setNextWayTarget();
            
                    return;
                }
                // using this to get connected waypoint data
                unsigned iter = 0;
                // otherwise, for all nodes the parent Node has connections to
                for (auto wpPair : pNode->waypoint->GetConnectedWaypoints())
                {
                    const Waypoint *wp = wpPair.first;
                    Node *cNode = &nodeList[wp->GetIndex()];
            
                    // compute cost of node (h(x) + g(x)
                    auto newHeuristic = CalculateHeurisitic(goalData->waypoint->GetPosition(), wp->GetPosition());

                    // var cost_so_far = pNode.cost_so_far + (1.2f * pNode.waypoint.conWaypointDistances[iter]);
                    auto newCostSoFar = pNode->costSoFar + (wp->GetCost() * 1.2f * wpPair.second);
            
                    auto priority = newHeuristic + newCostSoFar;
            
                    // if child is not on open or closed
                    if (!openList.Contains(cNode) && !cNode->onClosed)
                    {
                        cNode->hueristic = newHeuristic;

                        cNode->costSoFar = newCostSoFar;
                        
                        cNode->parent = pNode;

                        // place it on the open list
                        openList.Enqueue(cNode);

                        ++iter;

                        continue;
                    }

                    // if child is on open or closed list and new cost is cheaper
                    if (priority < cNode->GetPriority())
                    {
                        // if (m_open.Contains(cNode))
                        if (openList.Contains(cNode))
                        {
                            cNode->hueristic = newHeuristic;
                            
                            cNode->costSoFar = newCostSoFar;
                                 
                            cNode->parent = pNode;

                            openList.UpdateQueue();
                        }
                        
                        if (cNode->onClosed)
                        {
                            agent.GetClosedList().remove(cNode);

                            cNode->onClosed = false;
                        }
                        
                    }
                    ++iter;
                }

                // if we had a parent, remove anything on the closed list that comes before it
                if (pNode->parent != nullptr)
                {
                    Node *closedNode = closedList.back();

                    while (closedNode != pNode->parent)
                    {
                        closedList.pop_back();
                        closedNode = closedList.back();
                    }
                }

                // place parent node on closed list
                closedList.push_back(pNode);
            }


            //// if we get here, path is impossible, graph is incomplete
            //var debugMessage = "WAYPOINT PATHFINDING ERROR: path is impossible from: ("
            //    + start.x + ", " + start.y + ", " + start.z + ") to ("
            //    + goal.x + ", " + goal.y + ", " + goal.z + ")";
            //
            //Debug.LogError(debugMessage);
        }

        void WaypointSystem::OnInitialize(void)
        {
            m_world->Listener(this)
                .On(WORLD_ENTITY_COMPONENT_ADDED, &WaypointSystem::onComponentAdded)
                .On(WORLD_ENTITY_COMPONENT_REMOVED, &WaypointSystem::onComponentRemoved);
        }

        void WaypointSystem::OnRemove(void)
        {
            m_world->Listener( this )
                .Off( WORLD_ENTITY_COMPONENT_ADDED, &WaypointSystem::onComponentAdded )
                .Off( WORLD_ENTITY_COMPONENT_REMOVED, &WaypointSystem::onComponentRemoved );
        }

        void WaypointSystem::onComponentAdded(EVENT_HANDLER(World))
        {
            EVENT_ATTRS(World, ComponentEventArgs);

            if (args->component->Is<Waypoint>())
            {
                // add waypoint to our map of waypoints
                m_waypoints.emplace(
                    args->entity->GetUniqueID(),
                    static_cast<Waypoint*>( const_cast<Component*>( args->component ) )
                );

                // if it is set to auto connect, find everything it can connect to
                // ...
            }
        }

        void WaypointSystem::onComponentRemoved(EVENT_HANDLER(World))
        {
            EVENT_ATTRS(World, ComponentEventArgs);

            if (args->component->Is<Waypoint>())
            {
                auto search = m_waypoints.find(args->entity->GetUniqueID());

                if (search != m_waypoints.end())
                    m_waypoints.erase(search);
            }
        }

        float WaypointSystem::CalculateHeurisitic(const Vec3 &pos, const Vec3& goal)
        {
            return goal.DistanceSquared(pos);
        }
    }
}