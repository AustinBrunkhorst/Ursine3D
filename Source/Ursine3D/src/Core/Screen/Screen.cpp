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

#include "ScreenManager.h"

namespace ursine
{
    Screen::Screen(bool isOverlay)
        : EventDispatcher( this )
        , m_isOverlay( isOverlay )
        , m_isFocused( false )
        , m_state( SS_ACTIVE )
        , m_id( 0 ) { }

    void Screen::MessageUI(const std::string &message, const Json &data)
    {
        
    }

    void Screen::Exit(void)
    {
        
    }

    ScreenState Screen::GetState(void) const
    {
        return m_state;
    }

    ScreenID Screen::GetID(void) const
    {
        return m_id;
    }

    bool Screen::HasInputFocus(void) const
    {
        return m_isFocused;
    }

    void Screen::OnEntered(const Json &data)
    {
        
    }

    void Screen::setID(ScreenID id)
    {
        m_id = id;
    }
}
