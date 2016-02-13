#pragma once

#include "ResourceMetaData.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class ResourceItem
            {
            public:
                typedef std::shared_ptr<ResourceItem> Handle;

            private:
                std::string m_filename;
                GUID m_guid;
                ResourceMetaData m_metaData;
            };
        }
    }
}