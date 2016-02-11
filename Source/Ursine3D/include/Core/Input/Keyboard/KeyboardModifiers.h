/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** KeyboardModifiers.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <SDL_keycode.h>

namespace ursine
{
    enum KeyboardModifiers
    {
        KMD_NONE = KMOD_NONE,
        KMD_LSHIFT = KMOD_LSHIFT,
        KMD_RSHIFT = KMOD_RSHIFT,
        KMD_LCTRL = KMOD_LCTRL,
        KMD_RCTRL = KMOD_RCTRL,
        KMD_LALT = KMOD_LALT,
        KMD_RALT = KMOD_RALT,
        KMD_LGUI = KMOD_LGUI,
        KMD_RGUI = KMOD_RGUI,
        KMD_NUM = KMOD_NUM,
        KMD_CAPS = KMOD_CAPS,
        KMD_MODE = KMOD_MODE,
        KMD_CTRL = KMOD_CTRL,
        KMD_SHIFT = KMOD_SHIFT,
        KMD_ALT = KMOD_ALT,
        KMD_GUI = KMOD_GUI
    };
}