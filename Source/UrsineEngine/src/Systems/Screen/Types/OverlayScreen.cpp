/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** OverlayScreen.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "OverlayScreen.h"

namespace Ursine
{
    OverlayScreen::OverlayScreen(const std::string &name)
        : Screen(true, name) { }

    const std::string &OverlayScreen::GetName(void) const
    {
        return ui.GetName();
    }
}
