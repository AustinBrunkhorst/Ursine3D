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

                // A resource has completed building (either sucessfully or not)
                RP_BUILD_RESOURCE_COMPLETE,

                ///////////////////////////////////////////////////////////////

                // A resource has begun building a preview
                RP_BUILD_RESOURCE_PREVIEW_START,

                // A resource has completed building (either sucessfully or not)
                RP_BUILD_RESOURCE_PREVIEW_COMPLETE,

                ///////////////////////////////////////////////////////////////

                // All operations in a build pass have completed
                RP_BUILD_COMPLETE,

                ///////////////////////////////////////////////////////////////

                // A new resource has been added via file watching
                RP_RESOURCE_ADDED,

                // An existing resource has been removed
                RP_RESOURCE_REMOVED,
                
                // An existing resource has been modified via file watching
                RP_RESOURCE_MODIFIED,

                // An existing resource has been renamed
                RP_RESOURCE_RENAMED,
            };

            struct ResourceBuildArgs : EventArgs
            {
                ResourcePipelineEventType type { RP_BUILD_RESOURCE_START };
                ResourceItem::Handle resource { nullptr };
                TimeSpan operationDuration { TimeSpan::FromSeconds( 0 ) };

                // whether or not the build was sucessful
                bool successful { false };

                // Exception set if the build was not successful
                AssertionException error;

                // used when building on startup
                float progress { 0.0f };

                ResourceBuildArgs(void) = default;
            };

            struct ResourceChangeArgs : EventArgs
            {
                ResourcePipelineEventType type { RP_RESOURCE_ADDED };
                ResourceItem::Handle resource { nullptr };
                bool isGenerated { false };

                ResourceChangeArgs(void) = default;
            };

            struct ResourceRenameArgs : EventArgs
            {
                ResourceItem::Handle resource;
                fs::path oldName;

                ResourceRenameArgs(void) = default;
            };
        }
    }
}