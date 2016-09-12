/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ResourceMetaData.h
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
            struct ResourceMetaData
            {
                // ResourceImporter type that imports this resource
                meta::Type importer;

                // ResourceProcessor type that processes this resource
                meta::Type processor;

                // Options used by the processor
                Json processorOptions;

                // Options used to dictate directory behavior
                FileExclusionList syncExcludeExpressions;
            };
        }
    }
}