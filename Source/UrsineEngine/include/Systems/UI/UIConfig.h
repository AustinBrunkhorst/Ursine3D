/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Config.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

namespace ursine
{
    namespace UI
    {
        // Name of the global game instance in JavaScript
        const char * const kGameInstanceName = "Game";

        // Name of the function attached to the JavaScript game instance 
        // that triggers events with a JSON string
        const char * const kGameFnTrigger = "triggerJSON";
    }
}