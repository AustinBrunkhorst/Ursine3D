/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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
#include "UIScreenManager.h"

#include <mutex>

namespace ursine
{
    class JSUIScreenManager : public NativeJSClass
    {
        JAVASCRIPT_CLASS;

    public:
        JSConstructor(JSUIScreenManager);

    private:
        UIScreenManager *m_manager;

        std::mutex m_mutex;

        JSUIScreenManager(const JSUIScreenManager &rhs);
    } Meta(Enable, DisplayName( "NativeScreenManager" ));

    // This must be implmented in order to construct the UI level screen manager
    extern UIScreenManager *JSGetGlobalScreenManager(void);
}