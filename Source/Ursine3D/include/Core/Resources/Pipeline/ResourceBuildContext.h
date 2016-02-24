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
                ResourcePipelineManager *pipeline;

                ResourceItem::Handle resource;

                ResourceImportContext importContext;
                ResourceProcessorContext processorContext;

                ResourceImporter::Handle importer;
                ResourceProcessor::Handle processor;

                ResourceBuildContext(ResourcePipelineManager *pipeline, ResourceItem::Handle resource)
                    : pipeline( pipeline )
                    , resource( resource )
                    , importContext( pipeline, resource )
                    , processorContext( pipeline, resource ) { }
            };
        }
    }
}