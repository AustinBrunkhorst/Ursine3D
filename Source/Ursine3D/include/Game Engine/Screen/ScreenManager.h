/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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

#include "ScreenConfig.h"

#include "UIView.h"

namespace ursine
{
    class Screen;
    class OverlayScreen;

    class ScreenManager 
    {
    public:
        ScreenManager(void);
        ~ScreenManager(void);

        UIView::Handle GetUI(void) const;
        void SetUI(const UIView::Handle &ui);

        Screen *CreateScreen(const std::string &screenName);

        // Gets a screen with the given id. nullptr if invalid ID (doesn't exist)
        Screen *GetScreen(ScreenID id);

        // Attempts to get the first screen with the given name
        Screen *GetScreen(const std::string &screenName);

        // Sets the current non-overlay screen
        void SetScreen(Screen *screen, const Json &data = Json( ));

        // Adds an overlay screen above all other screens
        void AddOverlay(const std::string &name, const Json &data = Json( ));
        void AddOverlay(OverlayScreen *overlay, const Json &data = Json( ));

        // Removes a screen based on its ID
        void RemoveScreen(ScreenID id);

        // Removes a screen based on an instance
        void RemoveScreen(Screen *screen);

        // Removes the current non overlay screen if it exists
        void RemoveCurrent(void);

        // Messages a screen with the given message name and data
        void MessageScreen(ScreenID id, const std::string &message, const Json &data);

        // Gets the ID of the currently focused screen
        ScreenID GetFocusedScreen(void) const;

        void Update(void);

    private:
        friend class Screen;
        friend class Space;

        UIView::Handle m_ui;

        // next ID assigned to a screen
        ScreenID m_nextID;

        // current non-overlay screen
        Screen *m_current;

        // screen in queue to be set as current when "current" is actually removed
        std::pair<Screen *, Json> m_queued;

        // current overlay screens
        std::vector<Screen*> m_overlays;

        // queue of overlays to be removed
        std::vector<Screen*> m_overlayRemovalQueue;

        // screens mapped to their ID
        std::unordered_map<ScreenID, Screen*> m_map;
    } Meta(Register);

    extern ScreenManager *gScreenManager;
}
