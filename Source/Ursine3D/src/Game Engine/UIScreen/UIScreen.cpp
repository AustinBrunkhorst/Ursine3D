/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** UIScreen.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "UIScreen.h"

#include "UIScreenManager.h"

namespace
{
    const auto kChannelScreenManager = "ScreenManager";

    const auto kMessageEvent = "Event";
    const auto kMessageEntered = "Entered";
    const auto kMessageExited = "Exited";
}

namespace ursine
{
    UIScreen::UIScreen(UIScreenManager *manager, bool isInputBlocking, int priority)
        : EventDispatcher( this )
        , m_manager( manager )
        , m_isInputBlocking( isInputBlocking )
        , m_isFocused( false )
        , m_priority( priority )
        , m_state( SS_ACTIVE )
        , m_id( 0 ) { }

    void UIScreen::Message(const std::string &message, const Json &data) const
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

    void UIScreen::Exit(void) const
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

    UIScreenManager *UIScreen::GetManager(void) const
    {
        return m_manager;
    }

    UIScreenState UIScreen::GetState(void) const
    {
        return m_state;
    }

    UIScreenID UIScreen::GetID(void) const
    {
        return m_id;
    }

    bool UIScreen::HasInputFocus(void) const
    {
        return m_isFocused;
    }

    void UIScreen::OnEntered(const std::string &name, const Json &data)
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
}
