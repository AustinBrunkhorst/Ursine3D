/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ResourceDirectoryNode.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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

    const fs::path &rp::ResourceDirectoryNode::GetPath(void) const
    {
        return m_path;
    }

    const rp::ResourceDirectoryNode::List &rp::ResourceDirectoryNode::GetSubDirectories(void) const
    {
        return m_subDirectories;
    }

    const rp::ResourceItem::List &rp::ResourceDirectoryNode::GetResources(void) const
    {
        return m_resources;
    }
}
