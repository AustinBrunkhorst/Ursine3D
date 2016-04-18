/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** WorldProcessor.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "WorldProcessor.h"
#include "WorldData.h"

#include "JsonData.h"

namespace ursine
{
    rp::WorldProcessor::WorldProcessor(void) { }

    resources::ResourceData::Handle rp::WorldProcessor::Process(resources::ResourceData::Handle input, ResourceProcessorContext &context)
    {
        auto *json = expectInput<JsonData>( input );

        return std::make_shared<WorldData>( json->GetData( ) );
    }
}