#include "Precompiled.h"

#include "AIPriorityQueue.h"

#include <algorithm>

#define HeapArguments m_queue.begin(), m_queue.end(), comp

namespace ursine
{

    ////////////////////////////////////////
    //Node Class
    ////////////////////////////////////////
    Node::Node(ecs::Waypoint *wp, float fx, float hx) : waypoint(wp)
                                                 , parent(nullptr)
                                                 , costSoFar(fx)
                                                 , hueristic(hx)
                                                 , onClosed(false)
    {
    }
    
    bool Node::operator<(const Node& rhs) const
    {
        return hueristic + costSoFar < rhs.hueristic + costSoFar;
    }
    
    
    bool compare::operator()(NodePtr lhs, NodePtr rhs) const
    {
        return *lhs < *rhs;
    }
    
    float Node::GetPriority() const
    {
        return hueristic + costSoFar;
    }
    
    ////////////////////////////////////////
    //AIPriorityQueue Class
    ////////////////////////////////////////
    AIPriorityQueue::AIPriorityQueue()
    {
        compare comp;
    
        std::make_heap(HeapArguments);
    }
    
    unsigned AIPriorityQueue::GetCount() const
    {
        return static_cast<unsigned>(m_queue.size());
    }
    
    void AIPriorityQueue::Clear()
    {
        internalQueue empty;
        std::swap(m_queue, empty);
    }
    
    bool AIPriorityQueue::Contains(NodePtr node) const
    {
        // for now we are just going to loop past everything
        for (auto &qNode : m_queue)
        {
            if (qNode == node)
                return true;
        }
    
        return false;
    }
    
    
    
    void AIPriorityQueue::Enqueue(NodePtr node)
    {
        m_queue.push_back(node);
        compare comp;
    
        std::push_heap(HeapArguments);
    }
    
    NodePtr AIPriorityQueue::Dequeue(void)
    {
        compare comp;
        std::pop_heap(m_queue.begin(), m_queue.end(), comp);
    
        auto retval = m_queue.back();
    
        m_queue.pop_back();
    
        return retval;
    }
    
    bool AIPriorityQueue::IsEmpty(void) const
    {
        return m_queue.empty();
    }
    
    void AIPriorityQueue::UpdateQueue(void)
    {
        compare comp;
    
        std::sort_heap(HeapArguments);
    }
        
}
