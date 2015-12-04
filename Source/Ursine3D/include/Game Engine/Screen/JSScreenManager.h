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