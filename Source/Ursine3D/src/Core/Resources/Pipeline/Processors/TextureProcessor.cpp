/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** TextureProcessor.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "TextureProcessor.h"

namespace ursine
{
    rp::TextureProcessor::TextureProcessor(void) { }

    resources::ResourceData::Handle rp::TextureProcessor::Process(resources::ResourceData::Handle input, ResourceProcessorContext &context)
    {
        return input;
    }
}
