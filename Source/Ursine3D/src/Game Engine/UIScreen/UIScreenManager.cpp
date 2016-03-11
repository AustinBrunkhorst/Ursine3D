/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ScreenManager.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "UIScreenManager.h"

#include "UIScreen.h"

namespace ursine
{
    UIScreenManager::UIScreenManager(void)
        : m_nextID( 0 ) { }

    UIScreenManager::~UIScreenManager(void)
    {
        
    }

    UIView::Handle UIScreenManager::GetUI(void) const
    {
        return m_ui;
    }

    void UIScreenManager::SetUI(const UIView::Handle &ui)
    {
        m_ui = ui;
    }

    UIScreen *UIScreenManager::CreateScreen(
        const std::string &screenName,
        bool isInputBlocking,
        int priority
    )
    {
        // @@@TODO:
        return nullptr;
    }

    UIScreen *UIScreenManager::GetScreen(UIScreenID id)
    {
        auto screen = m_idToScreen.find( id );

        return screen == m_idToScreen.end( ) ? nullptr : screen->second;
    }

    UIScreen *UIScreenManager::GetScreen(const std::string &screenName)
    {
        auto search = m_nameToScreen.find( screenName );

        return search == m_nameToScreen.end( ) ? nullptr : search->second;
    }

    void UIScreenManager::RemoveScreen(UIScreen *screen)
    {
        // doesn't exist or already deleting
        if (!screen || screen->m_state == SS_DELETING)
            return;

        screen->m_state = SS_DELETING;

        screen->m_isFocused = false;

        m_removalQueue.push_back( screen );

        m_nameToScreen.erase( screen->m_name );
        m_idToScreen.erase( screen->m_id );
    }

    void UIScreenManager::MessageScreen(UIScreen *screen, const std::string &message, const Json &data)
    {
        if (!screen)
            return;

        UIScreenMessageArgs args( message, data );

        screen->Dispatch( message, &args );
    }
}
