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
        for (auto *screen : m_screens)
            delete screen;
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
        const std::string &name,
        bool isInputBlocking,
        int priority
    )
    {
        auto search = m_nameToScreen.find( name );

        UAssert( search == m_nameToScreen.end( ),
            "Screen '%s' already exists.",
            name.c_str( )
        );

        auto id = m_nextID++;

        auto *screen = new UIScreen(
            this, 
            id, 
            name, 
            isInputBlocking, 
            priority 
        );

        m_nameToScreen[ name ] = screen;
        m_idToScreen[ id ] = screen;

        utils::InsertionSort( m_screens, compareScreens );

        invalidateScreenFocus( );

        return screen;
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
        m_nameToScreen.erase( screen->m_name );
        m_idToScreen.erase( screen->m_id );

        auto search = find( m_screens.begin( ), m_screens.end( ), screen );

        if (search != m_screens.end( ))
            m_screens.erase( search );

        delete screen;

        invalidateScreenFocus( );
    }

    void UIScreenManager::MessageScreen(UIScreen *screen, const std::string &message, const Json &data)
    {
        if (!screen)
            return;

        UIScreenMessageArgs args( message, data );

        screen->Dispatch( message, &args );
    }

    bool UIScreenManager::compareScreens(const UIScreen *a, const UIScreen *b)
    {
        return b->m_id < a->m_id;
    }

    void UIScreenManager::invalidateScreenFocus(void)
    {
        auto hasFocus = true;

        // the first input blocking screen blocks focus to the rest
        // of the screens - because they're sorted, the higher priority
        // screens will be set to focus first
        for (auto *screen : m_screens)
        {
            screen->m_isFocused = hasFocus;

            if (hasFocus && screen->m_isInputBlocking)
                hasFocus = false;
        }
    }
}
