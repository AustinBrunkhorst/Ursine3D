/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** TextureProcessor.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ResourceProcessor.h"

#include "Options/TextureProcessorOptions.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class TextureProcessor : public ResourceProcessor
            {
                RESOURCE_PROCESSOR;

            public:
                TextureProcessor(void);

            private:
                ResourceData::Handle Process(ResourceData::Handle input, ResourceProcessorContext &context) override;
            } Meta(
                Enable,
                DisplayName( "TextureProcessor" ),
                ResourceProcessorConfig( 
                    typeof( ursine::rp::TextureProcessorOptions )
                )
            );
        }
    }
}