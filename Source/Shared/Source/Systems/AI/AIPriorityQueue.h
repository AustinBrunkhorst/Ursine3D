/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AIPriorityQueue.h
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/
//NOTE: This is currently a wrapper around std::priority_queue, which will be replaced
#pragma once

#include <vector>

#include "WaypointComponent.h"

namespace ursine
{

    struct Node
    {
        Node(ecs::Waypoint *wp, float fx, float hx);
    
        bool operator<(const Node& rhs) const;
    
        float GetPriority(void) const;
    
        ecs::Waypoint *waypoint;
    
        Node *parent;
    
        float costSoFar;
    
        float hueristic;
    
        bool onClosed;
    };
    
    typedef Node* NodePtr;
    
    struct compare
    {
        bool operator()(NodePtr lhs, NodePtr rhs) const;
    };
    
    // WARNING: THIS DOES NOT CALL DELETE ON NODES PAST AS A POINTER, THAT IS UP TO THE USER
    class AIPriorityQueue
    {
    public:
        //typedef std::priority_queue<Node *, std::vector<Node *>, Node::compare> internalQueue;
        typedef std::vector<NodePtr> internalQueue;
    
        AIPriorityQueue();
        //~AIPriorityQueue();
    
        unsigned GetCount(void) const;
    
        void Clear(void);
    
        bool Contains(NodePtr node) const;
    
        void Enqueue(NodePtr node);
    
        NodePtr Dequeue(void);
    
        bool IsEmpty(void) const;
    
        void UpdateQueue(void);
    
    private:
        internalQueue m_queue;
    };
    
    
    
}
