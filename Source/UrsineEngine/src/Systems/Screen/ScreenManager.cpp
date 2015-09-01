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

#include "UISystem.h"

#include "SpaceEvent.h"

namespace ursine
{
    ScreenManager *gScreenManager = nullptr;

    ScreenManager::ScreenManager(void)
        : _next_id(0)
        , _current(nullptr)
        , _queued({ nullptr, Json::object { } })
    {
        gScreenManager = this;
    }

    ScreenManager::~ScreenManager(void)
    {
        if (_current)
        {
            _current->OnRemoved();

            delete _current;
        }

        if (_queued.first)
            delete _queued.first;

        for (auto it = _overlays.rbegin(); it != _overlays.rend(); ++it)
        {
            auto overlay = *it;

            overlay->OnRemoved();

            delete overlay;
        }
    }

    void ScreenManager::Update(void)
    {
        auto no_overlays = _overlays.empty();

        // handle input only for first overlay (end of vector)
        if (!no_overlays)
        {
            auto end = _overlays.back();

            // only if active
            if (end->_state == SS_ACTIVE)
                end->HandleInput();
        }

        // update all overlays (back to front)
        for (auto it = _overlays.rbegin(); it != _overlays.rend(); ++it)
        {
            auto overlay = *it;

            if (overlay->_state == SS_ACTIVE)
                overlay->Update();
        }

        // attempt to update the current non-overlay screen
        if (_current && _current->_state == SS_ACTIVE)
        {
            // handle input only if no overlays exist
            if (no_overlays)
                _current->HandleInput();

            _current->Update();

            // non overlay is always drawn first
            _current->Draw();
        }

        // draw all overlays (front to back)
        for (auto overlay : _overlays)
            overlay->Draw();

        for (auto it = _overlay_removal_queue.rbegin(); it != _overlay_removal_queue.rend(); ++it)
        {
            auto screen = *it;

            // find the screen in the overlays vector 
            auto search = find(_overlays.begin(), _overlays.end(), screen);

            UAssert(search != _overlays.end(),
                "Overlay in removal queue doesn't exist.\nspace: %s",
                screen->ui.GetName().c_str());

            screen->OnRemoved();

            _overlays.erase(search);

            delete screen;
        }

        if (_overlay_removal_queue.size())
        {
            if (_overlays.empty())
            {
                // add focus to the current screen if applicable
                if (_current && !_current->_is_focused)
                {
                    _current->_is_focused = true;

                    _current->OnFocusChanged(true);
                }
            }
            else
            {
                auto last = _overlays.back();

                // add focus to the last overlay screen if applicable
                if (last->_state == SS_ACTIVE && !last->_is_focused)
                {
                    last->_is_focused = true;

                    last->OnFocusChanged(true);
                }
            }
        }

        _overlay_removal_queue.clear();

        if (_current && _current->_state == SS_DELETING)
        {
            _current->OnRemoved();

            delete _current;

            _current = _queued.first;
            _queued.first = nullptr;

            if (_current)
            {
                // focused if there are no overlays
                _current->_is_focused = _overlays.empty();

                _current->OnEntered(_queued.second);
            }
        }
    }

    Screen *ScreenManager::GetScreen(ScreenID id)
    {
        auto screen = _map.find(id);

        return screen == _map.end() ? nullptr : screen->second;
    }

    void ScreenManager::SetScreen(Screen *screen, const Json &data)
    {
        UAssert(!screen->_is_overlay,
            "Set screen must not be an overlay.");

        auto id = _next_id++;

        screen->setID(id);

        _map[id] = screen;

        // no screen currently, set straight as current
        if (!_current)
        {
            // focused if no overlays
            screen->_is_focused = _overlays.empty();

            screen->OnEntered(data);

            _current = screen;
        }
        // add it to the queue, exit current
        else
        {
            // something is already queued, just overwrite it
            if (_queued.first)
                delete _queued.first;

            _queued.first = screen;
            _queued.second = data;

            _current->Exit();
        }
    }

    void ScreenManager::AddOverlay(const std::string &name, const Json &data)
    {
        AddOverlay(new OverlayScreen(name), data);
    }

    void ScreenManager::AddOverlay(OverlayScreen *overlay, const Json &data)
    {
        auto id = _next_id++;

        overlay->setID(id);

        _map[id] = overlay;

        // clear focus from current screen if applicable
        if (_current && _current->_is_focused)
        {
            _current->_is_focused = false;

            _current->OnFocusChanged(false);
        }

        if (_overlays.size())
        {
            auto last = _overlays.back();

            // clear focus from last overlay screen if applicable
            if (last->_is_focused)
            {
                last->_is_focused = false;

                last->OnFocusChanged(false);
            }
        }

        _overlays.push_back(overlay);

        overlay->_is_focused = true;

        overlay->OnEntered(data);
    }

    void ScreenManager::RemoveScreen(ScreenID id)
    {
        RemoveScreen(GetScreen(id));
    }

    void ScreenManager::RemoveScreen(Screen *screen)
    {
        // doesn't exist or already deleting
        if (!screen || screen->_state == SS_DELETING)
            return;

        screen->_state = SS_DELETING;

        screen->_is_focused = false;

        if (screen->_is_overlay)
            _overlay_removal_queue.push_back(screen);

        if (screen == _queued.first)
        {
            delete _queued.first;

            _queued.first = nullptr;
        }

        _map.erase(screen->_id);
    }

    void ScreenManager::RemoveCurrent(void)
    {
        if (_current)
            _current->Exit();
    }

    void ScreenManager::MessageScreen(ScreenID id, const std::string &name, const Json &data)
    {
        auto screen = GetScreen(id);

        if (!screen)
            return;

        UI::SpaceMessageArgs args(name, data);

        screen->ui.Dispatch(name, &args);
    }

    ScreenID ScreenManager::GetFocusedScreen(void) const
    {
        if (_overlays.empty())
        {
            if (_current)
                return _current->_id;
            else
                return 0;
        }

        return _overlays.back()->_id;
    }

    Screen *ScreenManager::Create(const std::string &name)
    {
        auto search = _factories.find(name);

        UAssert(search != _factories.end(),
            "Unregistered screen factory \"%s\".", name.c_str());

        return search->second();
    }
}
