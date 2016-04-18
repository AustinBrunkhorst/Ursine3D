/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ResourceBuildCache.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class ResourceItem;

            struct ResourceBuildCache
            {
                // Determines if this asset built a preview
                bool hasPreview { false };

                // Runtime type of the processed resource
                meta::Type processedType { meta::Type::Invalid };

                // Resource that generated this resource.
                // NULL GUID if it doesn't exist
                GUID parent;

                // Resources that were generated from this resource
                std::unordered_set<GUID, GUIDHasher> generatedResources;
            };
        }
    }
}