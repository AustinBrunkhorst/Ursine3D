/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ArchetypeProcessor.h
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
            class ArchetypeProcessor : public ResourceProcessor
            {
                RESOURCE_PROCESSOR;

            public:
                ArchetypeProcessor(void);

            private:
                ResourceData::Handle Process(ResourceData::Handle input, ResourceProcessorContext &context) override;
            } Meta(
                Enable,
                DisplayName( "ArchetypeProcessor" )
            );
        }
    }
}