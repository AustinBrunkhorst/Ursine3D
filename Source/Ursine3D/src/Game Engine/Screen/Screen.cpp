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

namespace
{
    const auto kChannelScreenManager = "ScreenManager";

    const auto kMessageEvent = "Event";
    const auto kMessageEntered = "Entered";
    const auto kMessageExited = "Exited";
}

namespace ursine
{
    Screen::Screen(ScreenManager *manager, bool isOverlay)
        : EventDispatcher( this )
        , m_manager( manager )
        , m_isOverlay( isOverlay )
        , m_isFocused( false )
        , m_state( SS_ACTIVE )
        , m_id( 0 ) { }

    void Screen::MessageUI(const std::string &message, const Json &data) const
    {
        auto eventData = Json::object {
            { "screenID", static_cast<int>( m_id ) },
            { "event", message },
            { "data", data }
        };

        m_manager->GetUI( )->Message( 
            UI_CMD_BROADCAST, 
            kChannelScreenManager,
            kMessageEvent, 
            eventData
        );
    }

    void Screen::Exit(void) const
    {
        auto exitData = Json::object {
            { "screenID", static_cast<int>( m_id ) },
        };

        m_manager->GetUI( )->Message( 
            UI_CMD_BROADCAST, 
            kChannelScreenManager,
            kMessageExited, 
            exitData
        );
    }

    ScreenManager *Screen::GetManager(void) const
    {
        return m_manager;
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

    void Screen::OnEntered(const std::string &name, const Json &data)
    {
        auto enteredData = Json::object {
            { "screenName", name },
            { "screenID", static_cast<int>( m_id ) },
            { "data", data }
        };

        m_manager->GetUI( )->Message(
            UI_CMD_BROADCAST, 
            kChannelScreenManager,
            kMessageEntered, 
            enteredData
        );
    }

    void Screen::setID(ScreenID id)
    {
        m_id = id;
    }
}
