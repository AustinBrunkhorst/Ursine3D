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

#include "UIManager.h"

#include "ScreenManager.h"

namespace ursine
{
    Screen::Screen(bool is_overlay, const std::string &space_name)
        : _is_overlay(is_overlay)
        , _is_focused(false)
        , _state(SS_ACTIVE)
        , _id(0)
    {

    }

    void Screen::Exit(void)
    {
        
    }

    ScreenState Screen::GetState(void) const
    {
        return _state;
    }

    ScreenID Screen::GetID(void) const
    {
        return _id;
    }

    bool Screen::HasInputFocus(void) const
    {
        return _is_focused;
    }

    void Screen::OnEntered(const Json &data)
    {
        
    }

    void Screen::setID(ScreenID id)
    {
        _id = id;
    }
}