#include "UrsinePrecompiled.h"

#include "ResourceDirectoryNode.h"

namespace ursine
{
    rp::ResourceDirectoryNode::ResourceDirectoryNode(
        ResourceDirectoryNode *parent, 
        const fs::path &path /*= "" */
    )
        : m_path( path )
        , m_parent( parent ) { }

    rp::ResourceDirectoryNode::~ResourceDirectoryNode(void)
    {
        for (auto *subDirectory : m_subDirectories)
            delete subDirectory;
    }

    const fs::path &rp::ResourceDirectoryNode::GetPath(void)
    {
        return m_path;
    }

    const rp::ResourceDirectoryNode::List &rp::ResourceDirectoryNode::GetSubDirectories(void)
    {
        return m_subDirectories;
    }

    const rp::ResourceItem::List &rp::ResourceDirectoryNode::GetResources(void)
    {
        return m_resources;
    }
}
