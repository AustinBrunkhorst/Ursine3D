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

#include "Space.h"

#include "ScreenConfig.h"

namespace Ursine
{
    class Screen
    {
        friend class ScreenManager;

        // determines if this screen is an overlay
        bool _is_overlay;

        // determines if the screen is currently focused
        bool _is_focused;

        // current state of the screen
        ScreenState _state;

        // instance ID of the screen
        ScreenID _id;

        // internally sets the ID of this screen
        void setID(ScreenID id);

    protected:
        UI::Space ui;

        // Called when this screen becomes the "current" screen in the screen manager
        virtual void OnEntered(const Json &data);

        // Called when this screen is removed
        virtual void OnRemoved(void) {}

        // Called when this screen loses or gains focus state
        virtual void OnFocusChanged(bool state) {}

        // Called when input should be process for this screen
        virtual void HandleInput(void) {}

        // Called when update logic should be performed for this screen
        virtual void Update(void) {}

        // Called when things should be drawn in this screen
        virtual void Draw(void) {}
    public:
        Screen(bool is_overlay, const std::string &space_name);
        virtual ~Screen(void) {}

        // Request to exit this screen
        void Exit(void);

        // Gets the current state of the screen
        ScreenState GetState(void) const;

        // Gets the instance ID of this screen
        ScreenID GetID(void) const;

        // Gets the UI space for this screen
        UI::Space &GetUI(void);

        // Determines if this screen has input focus
        bool HasInputFocus(void) const;
    };
}