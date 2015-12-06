/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** JSGamepadManager.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "NativeJSClass.h"
#include "GamepadManager.h"

namespace ursine
{
    class JSGamepadManager : public NativeJSClass
    {
        JAVASCRIPT_CLASS;

    public:
        JSConstructor(JSGamepadManager);
        ~JSGamepadManager(void);

        JSMethod(numConnected) const;
        JSMethod(isConnected) const;

        JSMethod(isButtonDown) const;
        JSMethod(isButtonUp) const;

        JSMethod(isButtonTriggeredDown) const;
        JSMethod(isButtonTriggeredUp) const;

    private:
        GamepadManager *m_manager;

        CefRefPtr<CefBrowser> m_browser;

        void onGamepadButtonDown(EVENT_HANDLER(GamepadManager));
        void onGamepadButtonUp(EVENT_HANDLER(GamepadManager));
        void onGamepadConnected(EVENT_HANDLER(GamepadManager));
        void onGamepadDisconnected(EVENT_HANDLER(GamepadManager));
    } Meta(Enable, DisplayName( "GamepadManager" ));
}