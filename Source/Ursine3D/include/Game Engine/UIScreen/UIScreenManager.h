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

#include "UIView.h"

namespace ursine
{
    class UIScreen;

    class UIScreenManager
    {
    public:
        UIScreenManager(void);
        ~UIScreenManager(void);

        UIView::Handle GetUI(void) const;
        void SetUI(const UIView::Handle &ui);

        UIScreen *CreateScreen(
            const std::string &name, 
            bool isInputBlocking = true, 
            int priority = 0
        );

        // Attempts to get the screen
        UIScreen *GetScreen(UIScreenID id);

        // Attempts to get the screen with the given name
        UIScreen *GetScreen(const std::string &screenName);

        // Removes a screen based on an instance
        void RemoveScreen(UIScreen *screen);

        // Messages a screen with the given message name and data
        void MessageScreen(UIScreen *screen, const std::string &message, const Json &data);

    private:
        friend class UIScreen;

        UIView::Handle m_ui;

        // next ID assigned to a screen
        UIScreenID m_nextID;

        std::unordered_map<std::string, UIScreen*> m_nameToScreen;
        std::unordered_map<UIScreenID, UIScreen*> m_idToScreen;

        // current screens sorted based on priority
        std::vector<UIScreen*> m_screens;

        // descending order of ids
        static bool compareScreens(const UIScreen *a, const UIScreen *b);

        // assigns focus to all of the screens after priority has been changed
        // throughout all screens
        void invalidateScreenFocus(void);
    } Meta(Register, EnablePtrType);
}
