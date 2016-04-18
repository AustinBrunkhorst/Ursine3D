/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** PassThroughProcessor.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "PassThroughProcessor.h"

namespace ursine
{
    rp::PassThroughProcessor::PassThroughProcessor(void) { }

    resources::ResourceData::Handle rp::PassThroughProcessor::Process(resources::ResourceData::Handle input, ResourceProcessorContext &context)
    {
        return input;
    }
}