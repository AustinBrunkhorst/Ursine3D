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
                ResourceDirectoryNode(ResourceDirectoryNode *parent);
                ~ResourceDirectoryNode(void);

            private:
                std::string m_name;

                ResourceDirectoryNode *m_parent;

                std::vector<ResourceDirectoryNode *> m_subDirectories;
                std::vector<ResourceItem::Handle> m_resources;
            };
        }
    }
}