#pragma once

#include "ResourceImporter.h"
#include "ResourceProcessor.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            struct ResourceBuildContext
            {
                ResourceItem::Handle resource;

                ResourceImportContext importContext;
                ResourceProcessorContext processorContext;

                ResourceImporter::Handle importer;
                ResourceProcessor::Handle processor;

                ResourceBuildContext(ResourcePipelineManager *pipeline, ResourceItem::Handle resource)
                    : resource( resource )
                {
                    importContext.pipeline = pipeline;
                    processorContext.pipeline = pipeline;
                }
            };
        }
    }
}