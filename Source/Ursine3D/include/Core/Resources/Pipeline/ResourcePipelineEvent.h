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

                // All resources in a build pass have completed
                RP_BUILD_COMPLETE
            };

            struct ResourceBuildArgs : EventArgs
            {
                ResourcePipelineEventType type { RP_BUILD_RESOURCE_START };
                ResourceItem::Handle resource { nullptr };
                TimeSpan buildDuration { TimeSpan::FromSeconds( 0 ) };

                // used when building on startup
                float progress { 0.0f };

                ResourceBuildArgs(void) = default;
            };
        }
    }
}