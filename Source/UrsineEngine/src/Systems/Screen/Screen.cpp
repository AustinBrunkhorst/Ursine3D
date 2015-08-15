/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Screen.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/


#include "UrsinePrecompiled.h"

#include "Screen.h"

#include "UISystem.h"

#include "ScreenManager.h"

namespace Ursine
{
    Screen::Screen(bool is_overlay, const std::string &space_name)
        : _is_overlay(is_overlay)
        , _is_focused(false)
        , _state(SS_ACTIVE)
        , _id(0)
        , ui(space_name)
    {

    }

    void Screen::Exit(void)
    {
        ui.Exited();
    }

    ScreenState Screen::GetState(void) const
    {
        return _state;
    }

    ScreenID Screen::GetID(void) const
    {
        return _id;
    }

    UI::Space &Screen::GetUI(void)
    {
        return ui;
    }

    bool Screen::HasInputFocus(void) const
    {
        return _is_focused;
    }

    void Screen::OnEntered(const Json &data)
    {
        ui.Entered(data);
    }

    void Screen::setID(ScreenID id)
    {
        _id = id;

        ui._owner = id;
    }
}