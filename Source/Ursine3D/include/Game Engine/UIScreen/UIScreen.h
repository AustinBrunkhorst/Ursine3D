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

    class UIScreen
        : public meta::Object
        , public EventDispatcher<std::string>
    {
    public:
        UIScreen(UIScreenManager *manager, bool isInputBlocking, int priority);

        void Message(const std::string &message, const Json &data) const;

        // Request to exit this screen
        void Exit(void) const;

        UIScreenManager *GetManager(void) const;

        // Gets the current state of the screen
        UIScreenState GetState(void) const;

        // Gets the instance ID of this screen
        UIScreenID GetID(void) const;

        // Determines if this screen has input focus
        bool HasInputFocus(void) const;

    protected:
        // Called when this screen becomes the "current" screen in the screen manager
        void OnEntered(const std::string &name, const Json &data);

    private:
        friend class UIScreenManager;

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

        // name of the screen
        std::string m_name;
    } Meta(Disable);
}