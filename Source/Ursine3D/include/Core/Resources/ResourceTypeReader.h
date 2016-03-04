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