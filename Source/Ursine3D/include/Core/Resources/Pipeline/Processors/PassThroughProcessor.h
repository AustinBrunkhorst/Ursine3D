/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** PassThroughProcessor.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourceProcessor.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            // Processor that simply returns the input
            class PassThroughProcessor : public ResourceProcessor
            {
                RESOURCE_PROCESSOR;

            public:
                PassThroughProcessor(void);

            private:
                ResourceData::Handle Process(ResourceData::Handle input, ResourceProcessorContext &context) override;
            } Meta(
                Enable,
                DisplayName( "PassThroughProcessor" )
            );
        }
    }
}