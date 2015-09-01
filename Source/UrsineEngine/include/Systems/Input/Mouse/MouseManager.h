#pragma once

#include "System.h"

#include "MouseManagerEvent.h"

#include "UrsineTypes.h"

namespace ursine
{
    class MouseManager 
        : public System
        , public EventDispatcher<MouseManagerEvent>
    {
        friend class Application;

        uint32 _state_last;
        uint32 _state_current;

        // position of the mouse
        Vec2 _position;

        // change in position of the mouse since the last frame
        Vec2 _position_delta;

        void onButtonDown(EVENT_HANDLER(Application));
        void onButtonUp(EVENT_HANDLER(Application));

        void onMove(EVENT_HANDLER(Application));

        void onScroll(EVENT_HANDLER(Application));

    public:
        MouseManager(void);
        ~MouseManager(void);

        void Update(void) override;

        const Vec2 &GetPosition(void) const;
        const Vec2 &GetPositionDelta(void) const;

        bool IsButtonDown(MouseButton button) const;
        bool IsButtonUp(MouseButton button) const;
        
        bool IsButtonTriggeredDown(MouseButton button) const;
        bool IsButtonTriggeredUp(MouseButton button) const;

        void ShowCursor(bool show) const;
        void SetWarped(bool warped) const;
    };

    extern MouseManager *gMouseManager;
}