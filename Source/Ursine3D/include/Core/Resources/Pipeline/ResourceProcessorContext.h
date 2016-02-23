#pragma once

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class ResourcePipelineManager;

            struct ResourceProcessorContext
            {
                ResourcePipelineManager *pipeline;
            };
        }
    }
}