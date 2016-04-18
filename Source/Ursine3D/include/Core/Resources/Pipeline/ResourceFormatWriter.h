/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ResourceFormatWriter.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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