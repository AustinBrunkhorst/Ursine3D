#pragma once

#define RESOURCE_DATA META_OBJECT

#include "ResourceWriter.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class ResourceWriter;
        }

        class ResourceData : public meta::Object
        {
        public:
            typedef std::shared_ptr<ResourceData> Handle;
            
            // Writes this resource instance to its built form
            virtual void Write(pipeline::ResourceWriter &output) = 0;

            // Type of runtime reader to use when loading built content
            virtual meta::Type GetReaderType(void) = 0;
        } Meta(WhiteListMethods);
    }
}