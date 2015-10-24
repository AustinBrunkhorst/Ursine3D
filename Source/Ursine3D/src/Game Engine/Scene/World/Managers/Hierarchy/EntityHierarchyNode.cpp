#include "UrsinePrecompiled.h"

#include "EntityHierarchyNode.h"

namespace ursine
{
    namespace ecs
    {
        EntityHierarchyNode::EntityHierarchyNode(EntityID id)
            : m_id( id )
            , m_parent( -1 )
            , m_root( id ) { }

        void EntityHierarchyNode::Deactivate(void)
        {
            m_parent = -1;
            m_root = m_id;
        }

        EntityID EntityHierarchyNode::ID(void) const
        {
            return m_id;
        }

        EntityID EntityHierarchyNode::Parent(void) const
        {
            return m_parent;
        }

        void EntityHierarchyNode::SetParent(EntityID parent)
        {
            m_parent = parent;
        }

        EntityID EntityHierarchyNode::Root(void) const
        {
            return m_root;
        }

        void EntityHierarchyNode::SetRoot(EntityID root)
        {
            m_root = root;
        }
    }
}
