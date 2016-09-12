/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ResourceTypeReader.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourceData.h"
#include "ResourceReader.h"

#define RESOURCE_TYPE_READER META_OBJECT

namespace ursine
{
    namespace resources
    {
        class ResourceTypeReader : public meta::Object
        {
        public:
            typedef std::shared_ptr<ResourceTypeReader> Handle;

            virtual ResourceData::Handle Read(ResourceReader &input) = 0;
        } Meta(WhiteListMethods);
    }
}