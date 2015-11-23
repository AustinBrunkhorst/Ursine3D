/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Screen.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "ScreenConfig.h"

namespace ursine
{
    class ScreenManager;

    class Screen
        : public meta::Object
        , public EventDispatcher<std::string>
    {
    public:
        Screen(ScreenManager *manager, bool isOverlay);
        virtual ~Screen(void) { }

        void MessageUI(const std::string &message, const Json &data) const;

        // Request to exit this screen
        void Exit(void) const;

        ScreenManager *GetManager(void) const;

        // Gets the current state of the screen
        ScreenState GetState(void) const;

        // Gets the instance ID of this screen
        ScreenID GetID(void) const;

        // Determines if this screen has input focus
        bool HasInputFocus(void) const;

    protected:
        // Called when this screen becomes the "current" screen in the screen manager
        virtual void OnEntered(const std::string &name, const Json &data);

        // Called when this screen is removed
        virtual void OnRemoved(void) { }

        // Called when this screen loses or gains focus state
        virtual void OnFocusChanged(bool state) { }

        // Called when input should be process for this screen
        virtual void HandleInput(void) { }

        // Called when update logic should be performed for this screen
        virtual void Update(void) { }

        // Called when things should be drawn in this screen
        virtual void Render(void) { }

    private:
        friend class ScreenManager;

        ScreenManager *m_manager;

        // determines if this screen is an overlay
        bool m_isOverlay;

        // determines if the screen is currently focused
        bool m_isFocused;

        // current state of the screen
        ScreenState m_state;

        // instance ID of the screen
        ScreenID m_id;

        // internally sets the ID of this screen
        void setID(ScreenID id);
    } Meta(Disable);
}