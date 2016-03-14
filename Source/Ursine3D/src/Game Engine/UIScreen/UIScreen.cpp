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
    UIScreen::UIScreen(
        UIScreenManager *manager,
        UIScreenID id,
        const std::string &name,
        bool isInputBlocking, 
        int priority
    )
        : EventDispatcher( this )
        , m_manager( manager )
        , m_isInputBlocking( isInputBlocking )
        , m_isFocused( false )
        , m_priority( priority )
        , m_state( SS_ACTIVE )
        , m_id( id )
        , m_name( name ) { }

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

    const std::string &UIScreen::GetName(void) const
    {
        return m_name;
    }

    bool UIScreen::HasInputFocus(void) const
    {
        return m_isFocused;
    }
}
