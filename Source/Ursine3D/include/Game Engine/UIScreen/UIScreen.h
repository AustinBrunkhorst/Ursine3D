/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** UIScreen.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "UIScreenConfig.h"

namespace ursine
{
    class UIScreenManager;

    class UIScreen : public EventDispatcher<std::string>
    {
    public:
        void Message(const std::string &message, const Json &data) const;

        // Request to exit this screen
        void Exit(const Json &data = { }) const;

        UIScreenManager *GetManager(void) const;

        // Gets the current state of the screen
        UIScreenState GetState(void) const;

        // Gets the instance ID of this screen
        UIScreenID GetID(void) const;

        const fs::path &GetPath(void) const;

        // Determines if this screen has input focus
        bool HasInputFocus(void) const;

    private:
        friend class UIScreenManager;

        UIScreen(
            UIScreenManager *manager,
            UIScreenID id,
            const fs::path &path,
            const UIScreenConfig &config
        );

        UIScreenManager *m_manager;

        // determines if this screen is an overlay
        bool m_isInputBlocking;

        // determines if the screen is currently focused
        bool m_isFocused;

        // layer in which the screen resides on and also
        // order in which focus is distributed
        int m_priority;

        // current state of the screen
        UIScreenState m_state;

        // instance ID of the screen
        UIScreenID m_id;

        // qualifed path to the screen
        fs::path m_path;
    } Meta(Disable);
}