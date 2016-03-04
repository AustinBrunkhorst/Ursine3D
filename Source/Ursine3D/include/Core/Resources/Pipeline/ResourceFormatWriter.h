#pragma once

#include "ResourceItem.h"
#include "ResourceWriter.h"
#include "ResourceData.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class ResourceFormatWriter
            {
            public:
                ResourceFormatWriter(ResourceItem::Handle resourceItem);
                
                void Write(ResourceData::Handle resource);

            private:
                ResourceItem::Handle m_resourceItem;

                ResourceWriter m_writer;
            };
        }
    }
}