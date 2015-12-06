/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** NativeJSClass.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "NativeJSClass.h"
#include "UIConfig.h"

#include "ScreenManager.h"

namespace ursine
{
    void NativeJSClass::messageBrowser(const std::string &channel, const std::string &message, const Json &data) const
    {
        URSINE_TODO( "find better solution to sending message" );
        gScreenManager->GetUI( )->Message(
            UI_CMD_BROADCAST, 
            channel, 
            message, 
            data 
        );
    }
}