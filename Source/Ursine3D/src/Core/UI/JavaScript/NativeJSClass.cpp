/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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

namespace ursine
{
    NativeJSClass::NativeJSClass(void)
        : m_browser( CefV8Context::GetEnteredContext( )->GetBrowser( ) ) { }

    void NativeJSClass::messageBrowser(const std::string &channel, const std::string &message, const Json &data) const
    {
        UIView::Message(
            m_browser,
            UI_CMD_BROADCAST, 
            channel, 
            message, 
            data 
        );
    }
}