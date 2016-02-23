#pragma once

#include "ResourceData.h"
#include "ResourceImportContext.h"
#include "ResourcePipelineProperties.h"
#include "FileSystem.h"

#define RESOURCE_IMPORTER META_OBJECT

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class ResourceImporter : public meta::Object
            {
            public:
                typedef std::shared_ptr<ResourceImporter> Handle;

            private:
                friend class ResourcePipelineManager;

                virtual ResourceData::Handle Import(const fs::path &fileName, const ResourceImportContext &context) = 0;

                virtual bool BuildPreview(const fs::path &outputFileName, const Vec2 &maxDimensions)
                {
                    return false;
                }
            };
        }
    }
}