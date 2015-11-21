/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** OverlayScreen.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Screen.h"

namespace ursine
{
    class OverlayScreen
        : public Screen
    {
        NATIVE_SCREEN

    public:
        OverlayScreen(const std::string &name);
        virtual ~OverlayScreen(void) { }

        const std::string &GetName(void) const;

    private:
        std::string m_name;
    };
}