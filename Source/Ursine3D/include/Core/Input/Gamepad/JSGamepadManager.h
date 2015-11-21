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

        JSMethod(numConnected) const;
        JSMethod(isConnected) const;

        JSMethod(isButtonDown) const;
        JSMethod(isButtonUp) const;

        JSMethod(isButtonTriggeredDown) const;
        JSMethod(isButtonTriggeredUp) const;

    private:
        GamepadManager *m_manager;
    } Meta(Enable, DisplayName( "GamepadManager" ));
}