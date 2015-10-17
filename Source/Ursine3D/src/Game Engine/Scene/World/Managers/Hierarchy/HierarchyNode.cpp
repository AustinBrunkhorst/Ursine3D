#include "UrsinePrecompiled.h"

#include "HierarchyNode.h"
#include "Entity.h"

namespace ursine
{
    namespace ecs
    {
        const std::vector<EntityID> *HierarchyNode::Children() const
        {
            return &m_children;
        }

        void HierarchyNode::AddChild(EntityID child)
        {
            m_children.emplace_back( child );
        }

        void HierarchyNode::RemoveChild(EntityID child)
        {
            for (uint i = 0, s = m_children.size( ); i < s; ++i)
                if (m_children.at( i ) == child)
                {
                    m_children.erase( m_children.begin( ) + i );
                    return;
                }
        }
    }
}
