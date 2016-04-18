/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ResourceDirectoryNode.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourceItem.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class ResourceDirectoryNode
            {
            public:
                typedef std::vector<ResourceDirectoryNode*> List;

                ResourceDirectoryNode(ResourceDirectoryNode *parent, const fs::path &path = "");
                ~ResourceDirectoryNode(void);

                const fs::path &GetPath(void) const;

                const List &GetSubDirectories(void) const;
                const ResourceItem::List &GetResources(void) const;

            private:
                friend class ResourcePipelineManager;

                fs::path m_path;

                ResourceDirectoryNode *m_parent;

                std::vector<ResourceDirectoryNode *> m_subDirectories;
                std::vector<ResourceItem::Handle> m_resources;
            };
        }
    }
}