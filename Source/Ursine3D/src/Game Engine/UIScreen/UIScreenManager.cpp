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

namespace
{
    namespace channel
    {
        const auto ScreenManager = "ScreenManager";
    }

    namespace events
    {
        namespace manager
        {
            const auto ScreensCleared = "ScreensCleared";
        }

        namespace screen
        {
            const auto Added = "ScreenAdded";
            const auto Messaged = "ScreenMessaged";
            const auto Exited = "ScreenExited";
            const auto Removed = "ScreenRemoved";
        }
    }
}

namespace ursine
{
    UIScreenManager::UIScreenManager(void)
        : m_ui( nullptr )
        , m_nextID( 0 ) { }

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
        const fs::path &path, 
        const UIScreenConfig &config /*= { }*/,
        const Json &initData /*= { }*/
    )
    {
        auto *screen = allocateScreen( path, config );

        auto data = Json::object {
            { "path", path.string( ) },
            { "id", static_cast<int>( screen->m_id ) },
            { "priority", screen->m_priority },
            { "initData", initData }
        };

        messageUI( events::screen::Added, data );

        return screen;
    }

    UIScreen *UIScreenManager::CreateScreen(
        const resources::UIScreenData *resource,
        const UIScreenConfig &config /*= { }*/,
        const Json &initData /*= { }*/
    )
    {
        if (!resource)
            return nullptr;

        return CreateScreen( resource->GetQualifiedPath( ), config, initData );
    }

    UIScreen *UIScreenManager::CreateScreenRemote(
        const fs::path &path, 
        const UIScreenConfig &config
    )
    {
        return allocateScreen( path, config );
    }

    UIScreen *UIScreenManager::GetScreen(UIScreenID id)
    {
        auto screen = m_idToScreen.find( id );

        return screen == m_idToScreen.end( ) ? nullptr : screen->second;
    }

    UIScreen *UIScreenManager::GetScreen(const fs::path &path)
    {
        auto search = m_pathToScreen.find( path );

        return search == m_pathToScreen.end( ) ? nullptr : search->second;
    }

    void UIScreenManager::ExitScreen(const UIScreen *screen, const Json &exitData)
    {
        auto data = Json::object {
            { "id", static_cast<int>( screen->m_id ) },
            { "data", exitData }
        };

        messageUI( events::screen::Exited, data );
    }

    void UIScreenManager::RemoveScreen(UIScreen *screen)
    {
        m_pathToScreen.erase( screen->m_path );
        m_idToScreen.erase( screen->m_id );

        auto search = find( m_screens.begin( ), m_screens.end( ), screen );

        if (search != m_screens.end( ))
            m_screens.erase( search );

        delete screen;

        invalidateScreenFocus( );

        auto data = Json::object {
            { "id", static_cast<int>( screen->m_id ) },
        };

        messageUI( events::screen::Removed, data );
    }

    void UIScreenManager::MessageScreenNative(
        const UIScreen *screen, 
        const std::string &message, 
        const Json &data
    ) const
    {
        if (!screen)
            return;

        UIScreenMessageArgs args( message, data );

        screen->Dispatch( message, &args );
    }

    void UIScreenManager::MessageScreenRemote(
        const UIScreen *screen, 
        const std::string &message, 
        const Json &data
    ) const
    {
        if (!screen)
            return;

        auto messageData = Json::object {
            { "id", static_cast<int>( screen->m_id ) },
            { "message", message },
            { "data", data }
        };

        messageUI( events::screen::Messaged, messageData );
    }

    void UIScreenManager::ClearScreens(void)
    {
        m_pathToScreen.clear( );
        m_idToScreen.clear( );

        for (auto *screen : m_screens)
            delete screen;

        m_screens.clear( );

        messageUI( events::manager::ScreensCleared, { } );
    }

    bool UIScreenManager::compareScreens(const UIScreen *a, const UIScreen *b)
    {
        return b->m_id < a->m_id;
    }

    UIScreen *UIScreenManager::allocateScreen(const fs::path &path, const UIScreenConfig &config)
    {
        auto id = m_nextID++;

        auto *screen = new UIScreen(
            this, 
            id, 
            path, 
            config
        );

        m_pathToScreen[ path ] = screen;
        m_idToScreen[ id ] = screen;

        utils::InsertionSort( m_screens, screen, compareScreens );

        invalidateScreenFocus( );

        return screen;
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

    void UIScreenManager::messageUI(const std::string &message, const Json &data) const
    {
        UAssert( m_ui,
            "UI was null."
        );

        m_ui->Message( 
            UI_CMD_BROADCAST,
            channel::ScreenManager,
            message, 
            data 
        );
    }
}
