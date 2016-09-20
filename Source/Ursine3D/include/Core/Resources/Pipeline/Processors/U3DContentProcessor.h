/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** U3DContentProcessor.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourceProcessor.h"

#include "Options/U3DContentProcessorOptions.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class U3DContentProcessor : public ResourceProcessor
            {
                RESOURCE_PROCESSOR;

            public:
                U3DContentProcessor(void);

            private:
                ResourceData::Handle Process(ResourceData::Handle input, ResourceProcessorContext &context) override;
            } Meta(
                Enable,
                DisplayName( "U3DContentProcessor" ),
                ResourceProcessorConfig(
                    typeof( ursine::rp::U3DContentProcessorOptions )
                )
            );
        }
    }
}
