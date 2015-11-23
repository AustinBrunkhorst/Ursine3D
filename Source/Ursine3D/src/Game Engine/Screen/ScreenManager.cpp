/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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

#include "ScreenManager.h"

#include "Screen.h"
#include "OverlayScreen.h"

namespace ursine
{
    ScreenManager *gScreenManager = nullptr;

    ScreenManager::ScreenManager(void)
        : m_nextID( 0 )
        , m_current( nullptr )
        , m_queued( { nullptr, Json::object { } } )
    {
        gScreenManager = this;
    }

    ScreenManager::~ScreenManager(void)
    {
        if (m_current)
        {
            m_current->OnRemoved( );

            delete m_current;
        }

        if (m_queued.first)
            delete m_queued.first;

        for (auto it = m_overlays.rbegin( ); it != m_overlays.rend( ); ++it)
        {
            auto overlay = *it;

            overlay->OnRemoved( );

            delete overlay;
        }
    }

    UIView::Handle ScreenManager::GetUI(void) const
    {
        return m_ui;
    }

    void ScreenManager::SetUI(const UIView::Handle &ui)
    {
        m_ui = ui;
    }

    Screen *ScreenManager::CreateScreen(const std::string &screenName)
    {
        auto screenType = meta::Type::GetFromName( screenName );

        UAssert( screenType.IsValid( ),
            "Unknown screen type '%s'.",
            screenName.c_str( )
        );

        const auto ctor = screenType.GetDynamicConstructor( {
            typeof( ScreenManager* ) 
        } );

        UAssert( ctor.IsValid( ),
            "Screen missing dynamic constructor.\n%s(ursine::ScreenManager *)",
            screenType.GetName( ).c_str( )
        );

        return ctor.Invoke( this ).GetValue<Screen*>( );
    }

    Screen *ScreenManager::GetScreen(ScreenID id)
    {
        auto screen = m_map.find( id );

        return screen == m_map.end( ) ? nullptr : screen->second;
    }

    void ScreenManager::SetScreen(Screen *screen, const Json &data)
    {
        UAssert( !screen->m_isOverlay,
            "Set screen must not be an overlay." );

        auto id = m_nextID++;

        screen->setID( id );

        m_map[ id ] = screen;

        // no screen currently, set straight as current
        if (!m_current)
        {
            // focused if no overlays
            screen->m_isFocused = m_overlays.empty( );

            screen->OnEntered( screen->GetType( ).GetName( ), data );

            m_current = screen;
        }
        // add it to the queue, exit current
        else
        {
            // something is already queued, just overwrite it
            if (m_queued.first)
                delete m_queued.first;

            m_queued.first = screen;
            m_queued.second = data;

            m_current->Exit( );
        }
    }

    void ScreenManager::AddOverlay(const std::string &name, const Json &data)
    {
        AddOverlay( new OverlayScreen( this, name ), data );
    }

    void ScreenManager::AddOverlay(OverlayScreen *overlay, const Json &data)
    {
        auto id = m_nextID++;

        overlay->setID( id );

        m_map[ id ] = overlay;

        // clear focus from current screen if applicable
        if (m_current && m_current->m_isFocused)
        {
            m_current->m_isFocused = false;

            m_current->OnFocusChanged( false );
        }

        if (m_overlays.size( ))
        {
            auto last = m_overlays.back( );

            // clear focus from last overlay screen if applicable
            if (last->m_isFocused)
            {
                last->m_isFocused = false;

                last->OnFocusChanged( false );
            }
        }

        m_overlays.push_back( overlay );

        overlay->m_isFocused = true;

        overlay->OnEntered( overlay->GetName( ), data );
    }

    void ScreenManager::RemoveScreen(ScreenID id)
    {
        RemoveScreen( GetScreen( id ) );
    }

    void ScreenManager::RemoveScreen(Screen *screen)
    {
        // doesn't exist or already deleting
        if (!screen || screen->m_state == SS_DELETING)
            return;

        screen->m_state = SS_DELETING;

        screen->m_isFocused = false;

        if (screen->m_isOverlay)
            m_overlayRemovalQueue.push_back( screen );

        if (screen == m_queued.first)
        {
            delete m_queued.first;

            m_queued.first = nullptr;
        }

        m_map.erase( screen->m_id );
    }

    void ScreenManager::RemoveCurrent(void)
    {
        if (m_current)
            m_current->Exit( );
    }

    void ScreenManager::MessageScreen(ScreenID id, const std::string &message, const Json &data)
    {
        auto screen = GetScreen( id );

        if (!screen)
            return;

        UIMessageArgs args( message, data );

        screen->Dispatch( message, &args );
    }

    ScreenID ScreenManager::GetFocusedScreen(void) const
    {
        if (m_overlays.empty( ))
        {
            if (m_current)
                return m_current->m_id;

            return 0;
        }

        return m_overlays.back( )->m_id;
    }

    void ScreenManager::Update(void)
    {
        auto noOverlays = m_overlays.empty( );

        // handle input only for first overlay (end of vector)
        if (!noOverlays)
        {
            auto end = m_overlays.back( );

            // only if active
            if (end->m_state == SS_ACTIVE)
                end->HandleInput( );
        }

        // update all overlays (back to front)
        for (auto it = m_overlays.rbegin( ); it != m_overlays.rend( ); ++it)
        {
            auto overlay = *it;

            if (overlay->m_state == SS_ACTIVE)
                overlay->Update( );
        }

        // attempt to update the current non-overlay screen
        if (m_current && m_current->m_state == SS_ACTIVE)
        {
            // handle input only if no overlays exist
            if (noOverlays)
                m_current->HandleInput( );

            m_current->Update( );

            // non overlay is always drawn first
            m_current->Render( );
        }

        // draw all overlays (front to back)
        for (auto overlay : m_overlays)
            overlay->Render( );

        for (auto it = m_overlayRemovalQueue.rbegin( ); it != m_overlayRemovalQueue.rend( ); ++it)
        {
            auto screen = *it;

            // find the screen in the overlays vector 
            auto search = find( m_overlays.begin( ), m_overlays.end( ), screen );

            UAssert( search != m_overlays.end( ),
                "Overlay in removal queue doesn't exist.\nscreen: %s",
                screen->GetType( ).GetName( ).c_str( )
            );

            screen->OnRemoved( );

            m_overlays.erase( search );

            delete screen;
        }

        if (m_overlayRemovalQueue.size( ))
        {
            if (m_overlays.empty( ))
            {
                // add focus to the current screen if applicable
                if (m_current && !m_current->m_isFocused)
                {
                    m_current->m_isFocused = true;

                    m_current->OnFocusChanged( true );
                }
            }
            else
            {
                auto last = m_overlays.back( );

                // add focus to the last overlay screen if applicable
                if (last->m_state == SS_ACTIVE && !last->m_isFocused)
                {
                    last->m_isFocused = true;

                    last->OnFocusChanged( true );
                }
            }
        }

        m_overlayRemovalQueue.clear( );

        if (m_current && m_current->m_state == SS_DELETING)
        {
            m_current->OnRemoved( );

            delete m_current;

            m_current = m_queued.first;
            m_queued.first = nullptr;

            if (m_current)
            {
                // focused if there are no overlays
                m_current->m_isFocused = m_overlays.empty( );

                m_current->OnEntered( m_current->GetType( ).GetName( ), m_queued.second );
            }
        }

        if (m_ui)
            m_ui->DrawMain( );
    }
}
