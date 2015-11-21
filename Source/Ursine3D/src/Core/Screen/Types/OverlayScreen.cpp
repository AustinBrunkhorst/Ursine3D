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

namespace ursine
{
    OverlayScreen::OverlayScreen(const std::string &name)
        : Screen( true )
        , m_name( name ) { }

    const std::string &OverlayScreen::GetName(void) const
    {
        return m_name;
    }
}
