/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ScreenManager.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "System.h"

#include "ScreenConfig.h"

namespace Ursine
{
    class Screen;
    class OverlayScreen;

    namespace UI
    {
        class Space;
    }

    class ScreenManager : public System
    {
        friend class Screen;
        friend class Space;

        // next ID assigned to a screen
        ScreenID _next_id;

        // current non-overlay screen
        Screen *_current;

        // screen in queue to be set as current when "current" is actually removed
        std::pair<Screen *, Json> _queued;

        // current overlay screens
        std::vector<Screen*> _overlays;

        // queue of overlays to be removed
        std::vector<Screen*> _overlay_removal_queue;

        // screens mapped to their ID
        std::unordered_map<ScreenID, Screen*> _map;

        std::unordered_map<std::string, ScreenFactoryCallback> _factories;
    public:
        ScreenManager(void);
        ~ScreenManager(void);

        void Update(void) override;

        // Gets a screen with the given id. nullptr if invalid ID (doesn't exist)
        Screen *GetScreen(ScreenID id);

        // Sets the current non-overlay screen
        void SetScreen(Screen *screen, const Json &data = Json());

        // Adds an overlay screen above all other screens
        void AddOverlay(const std::string &name, const Json &data = Json());
        void AddOverlay(OverlayScreen *overlay, const Json &data = Json());

        // Removes a screen based on its ID
        void RemoveScreen(ScreenID id);

        // Removes a screen based on an instance
        void RemoveScreen(Screen *screen);

        // Removes the current non overlay screen if it exists
        void RemoveCurrent(void);

        // Messages a screen with the given message name and data
        void MessageScreen(ScreenID id, const std::string &name, const Json &data);

        // Gets the ID of the currently focused screen
        ScreenID GetFocusedScreen(void) const;

        template<typename ScreenType>
        void RegisterScreenFactory(const std::string &name);

        // Creates a screen with the given factory name
        Screen *Create(const std::string &name);
    };

    extern ScreenManager *gScreenManager;
}

#include "ScreenManager.hpp"