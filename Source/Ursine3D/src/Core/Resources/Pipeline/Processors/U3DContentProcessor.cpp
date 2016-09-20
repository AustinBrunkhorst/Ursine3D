/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** U3DContentProcessor.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "U3DContentProcessor.h"

namespace ursine
{
    rp::U3DContentProcessor::U3DContentProcessor(void) { }

    resources::ResourceData::Handle rp::U3DContentProcessor::Process(ResourceData::Handle input, ResourceProcessorContext& context)
    {
        // TODO: [J] How do I get the config?
        return input;
    }

}
