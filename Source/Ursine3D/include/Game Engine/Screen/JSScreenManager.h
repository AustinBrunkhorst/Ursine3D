/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** JSScreenManager.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "NativeJSClass.h"
#include "ScreenManager.h"

#include <mutex>

namespace ursine
{
    class JSScreenManager : public NativeJSClass
    {
        JAVASCRIPT_CLASS;

    public:
        JSConstructor(JSScreenManager);

        JSMethod(setScreen);
        JSMethod(addOverlay);
        JSMethod(removeScreen);
        JSMethod(removeCurrent);
        JSMethod(messageScreen);
        JSMethod(getFocusedScreen);

    private:
        ScreenManager *m_manager;

        std::mutex m_mutex;

        JSScreenManager(const JSScreenManager &rhs) { }
    } Meta(Enable, DisplayName( "ScreenManager" ));
}