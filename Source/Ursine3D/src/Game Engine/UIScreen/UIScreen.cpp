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

namespace ursine
{
    UIScreen::UIScreen(
        UIScreenManager *manager,
        UIScreenID id,
        const fs::path &path,
        const UIScreenConfig &config
    )
        : EventDispatcher( this )
        , m_manager( manager )
        , m_isInputBlocking( config.inputBlocking )
        , m_isFocused( false )
        , m_priority( config.priority )
        , m_state( SS_ACTIVE )
        , m_id( id )
        , m_path( path ) { }

    void UIScreen::Message(const std::string &message, const Json &data) const
    {
        m_manager->MessageScreenRemote( this, message, data );
    }

    void UIScreen::Exit(const Json &data /*= { }*/) const
    {
        m_manager->ExitScreen( this, data );
    }

    void UIScreen::Remove(void)
    {
        m_manager->RemoveScreen( this );
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

    const fs::path &UIScreen::GetPath(void) const
    {
        return m_path;
    }

    bool UIScreen::HasInputFocus(void) const
    {
        return m_isFocused;
    }
}
