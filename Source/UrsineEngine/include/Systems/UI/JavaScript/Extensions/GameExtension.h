/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GameExtension.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "JavaScriptExtension.h"

#include <mutex>

namespace Ursine
{
    class AudioEmitter;

    namespace UI
    {
        class GameExtension : public JavaScriptExtension
        {
            std::mutex _screen_manager_mutex;
            std::mutex _timer_manager_mutex;

            ////////////////////////////////////////////////////////////////////
            // General Methods
            ////////////////////////////////////////////////////////////////////

            JSMethod(ready);
            JSMethod(exit);

            JSMethod(showCursor);
            JSMethod(setCursorWarped);

            JSMethod(getAvailableDisplayModes);
            JSMethod(getDisplayMode);
            JSMethod(setDisplayMode);
            JSMethod(isFullScreen);
            JSMethod(setFullScreen);

            JSMethod(getTime);

            ////////////////////////////////////////////////////////////////////
            // Audio Methods
            ////////////////////////////////////////////////////////////////////

            JSMethod(audioIsPlaying);
            JSMethod(audioPlay);
            JSMethod(audioStop);
            JSMethod(audioGetVolume);
            JSMethod(audioSetVolume);

            ////////////////////////////////////////////////////////////////////
            // Screen Manager Methods
            ////////////////////////////////////////////////////////////////////

            JSMethod(setScreen);
            JSMethod(addOverlay);
            JSMethod(removeScreen);
            JSMethod(removeCurrent);
            JSMethod(messageScreen);
            JSMethod(getFocusedScreen);

            ////////////////////////////////////////////////////////////////////
            // Gamepad Manager Methods
            ////////////////////////////////////////////////////////////////////

            JSMethod(getConnectedGamepads);
            JSMethod(gamepadVibrate);
        public:
            GameExtension(void);
            ~GameExtension(void);

            void Register(void) override;
        };
    }
}