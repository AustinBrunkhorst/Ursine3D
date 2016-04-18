/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** UIScreenProcessor.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "UIScreenProcessor.h"

#include "JsonData.h"
#include "UIScreenData.h"

namespace ursine
{
    rp::UIScreenProcessor::UIScreenProcessor(void) { }

    resources::ResourceData::Handle rp::UIScreenProcessor::Process(resources::ResourceData::Handle input, ResourceProcessorContext &context)
    {
        auto *json = expectInput<JsonData>( input );

        return std::make_shared<UIScreenData>( json->GetData( ) );
    }
}