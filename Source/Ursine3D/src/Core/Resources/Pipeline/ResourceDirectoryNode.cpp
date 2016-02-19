#include "UrsinePrecompiled.h"

#include "ResourceDirectoryNode.h"

namespace ursine
{
    rp::ResourceDirectoryNode::ResourceDirectoryNode(ResourceDirectoryNode *parent)
        : m_parent( parent )
    {

    }

    rp::ResourceDirectoryNode::~ResourceDirectoryNode(void)
    {
        for (auto *subDirectory : m_subDirectories)
            delete subDirectory;
    }
}
