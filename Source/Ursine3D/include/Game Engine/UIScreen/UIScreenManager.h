/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** UIScreenManager.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "UIScreenConfig.h"
#include "UIScreenData.h"

#include "UIView.h"

namespace ursine
{
    class UIScreen;

    class UIScreenManager : public EventDispatcher<std::string>
    {
    public:
        UIScreenManager(void);
        ~UIScreenManager(void);

        UIView::Handle GetUI(void) const;
        void SetUI(const UIView::Handle &ui);

        // Creates a screen based on the given path
        UIScreen *CreateScreen(
            const fs::path &path,
            const UIScreenConfig &config = { },
            const Json &initData = { }
        );

        // Creates a screen based on a resource
        UIScreen *CreateScreen(
            const resources::UIScreenData *resource,
            const UIScreenConfig &config = { },
            const Json &initData = { }
        );

        // Creates a screen remotely, i.e. assumes that the scripting
        // side already knows about it
        UIScreen *CreateScreenRemote(
            const fs::path &path,
            const UIScreenConfig &config = { }
        );

        // Attempts to get the screen
        UIScreen *GetScreen(UIScreenID id);

        // Attempts to get the screen with the given path
        UIScreen *GetScreen(const fs::path &path);

        // Requests that the screen be removed
        void ExitScreen(const UIScreen *screen, const Json &exitData);

        // Removes a screen
        void RemoveScreen(UIScreen *screen);

        // Dispatches an event to the native screen events
        void MessageScreenNative(
            const UIScreen *screen, 
            const std::string &message, 
            const Json &data
        ) const;

        // Dispatches an event to the scripting side of things
        void MessageScreenRemote(
            const UIScreen *screen, 
            const std::string &message, 
            const Json &data
        ) const;

        // Dispatches an event globally to this manager
        void MessageGlobalNative(
            const std::string &message,
            const Json &data
        );

        void MessageGlobalRemote(
            const std::string &message,
            const Json &data
        );

        // Removes all screens
        void ClearScreens(void);

    private:
        friend class UIScreen;

        UIView::Handle m_ui;

        // next ID assigned to a screen
        UIScreenID m_nextID;

        fs::PathMap<UIScreen*> m_pathToScreen;
        std::unordered_map<UIScreenID, UIScreen*> m_idToScreen;

        // current screens sorted based on priority
        std::vector<UIScreen*> m_screens;

        // disable assignment
        UIScreenManager &operator=(const UIScreenManager &rhs) = delete;

        // descending order of ids
        static bool compareScreens(const UIScreen *a, const UIScreen *b);

        UIScreen *allocateScreen(
            const fs::path &path, 
            const UIScreenConfig &config
        );

        // assigns focus to all of the screens after priority has been changed
        // throughout all screens
        void invalidateScreenFocus(void);

        void messageUI(const std::string &message, const Json &data) const;
    } Meta(Register, EnablePtrType);
}
