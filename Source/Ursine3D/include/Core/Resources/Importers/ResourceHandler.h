#pragma once

#define RESOURCE_HANDLER META_OBJECT
#define RESOURCE_HANDLER_DEFINITION(type)

#include "ResourceHandlerMeta.h"

namespace ursine
{
    class ResourceImportContext;
    class ResourceOutputContext;
    class ResourceData;

    class ResourceHandler : public meta::Object
    {
    public:
        // Imports an asset from its original content
        virtual ResourceData *Import(ResourceImportContext &context) = 0;

        // Writes an imported asset to the output file
        virtual void WriteData(ResourceData *data, ResourceOutputContext &context) = 0;

        // Loads an imported asset from its output file
        virtual ResourceData *LoadData(ResourceOutputContext &context) = 0;
    } Meta(Enable, WhiteListMethods);
}