#pragma once

#include "EventArgs.h"

#include "ResourceItem.h"
#include "TimeSpan.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            enum ResourcePipelineEventType
            {
                // A resource has begun building
                RP_BUILD_RESOURCE_START,

                // A resource has completed building unsucessfully
                RP_BUILD_RESOURCE_FAIL,

                // A resource has completed building sucessfully
                RP_BUILD_RESOURCE_COMPLETE,

                ///////////////////////////////////////////////////////////////

                // A resource has begun building a preview
                RP_BUILD_RESOURCE_PREVIEW_START,

                // A resource has completed building a preview unsucessfully
                RP_BUILD_RESOURCE_PREVIEW_FAIL,

                // A resource has completed building a preview sucessfully
                RP_BUILD_RESOURCE_PREVIEW_COMPLETE,

                ///////////////////////////////////////////////////////////////

                // All operations in a build pass have completed
                RP_BUILD_COMPLETE
            };

            struct ResourceBuildArgs : EventArgs
            {
                ResourcePipelineEventType type { RP_BUILD_RESOURCE_START };
                ResourceItem::Handle resource { nullptr };
                TimeSpan operationDuration { TimeSpan::FromSeconds( 0 ) };

                // used when building on startup
                float progress { 0.0f };

                ResourceBuildArgs(void) = default;
            };
        }
    }
}