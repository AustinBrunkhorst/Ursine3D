/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** JSKeyboardManager.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "NativeJSClass.h"
#include "KeyboardManager.h"

namespace ursine
{
    class JSKeyboardManager : public NativeJSClass
    {
        JAVASCRIPT_CLASS;

    public:
        JSConstructor(JSKeyboardManager);
        ~JSKeyboardManager(void);

        JSMethod(isKeyDown) const;
        JSMethod(isKeyUp) const;

        JSMethod(isKeyTriggeredDown) const;
        JSMethod(isKeyTriggeredUp) const;

    private:
        KeyboardManager *m_manager;

        void onKeyboardKeyDown(EVENT_HANDLER(KeyboardManager));
        void onKeyboardKeyUp(EVENT_HANDLER(KeyboardManager));
    } Meta(Enable, DisplayName( "KeyboardManager" ));
}