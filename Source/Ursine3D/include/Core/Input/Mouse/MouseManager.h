#pragma once

#include "CoreSystem.h"

#include "MouseManagerEvent.h"

#include "UrsineTypes.h"

namespace ursine
{
    class MouseManager 
        : public core::CoreSystem
        , public EventDispatcher<MouseManagerEvent>
    {
        CORE_SYSTEM
    public:
        MouseManager(void);
        ~MouseManager(void);

        Meta(Disable)
        void OnInitialize(void) override;

        Meta(Disable)
        void OnRemove(void) override;

        Meta(Disable)
        const Vec2 &GetPosition(void) const;

        Meta(Disable)
        const Vec2 &GetPositionDelta(void) const;

        Meta(Disable)
        bool IsButtonDown(MouseButton button) const;

        Meta(Disable)
        bool IsButtonUp(MouseButton button) const;
        
        Meta(Disable)
        bool IsButtonTriggeredDown(MouseButton button) const;

        Meta(Disable)
        bool IsButtonTriggeredUp(MouseButton button) const;

        Meta(Disable)
        void ShowCursor(bool show) const;

        Meta(Disable)
        void SetWarped(bool warped) const;

    private:
        friend class Application;

        uint32 m_stateLast;
        uint32 m_stateCurrent;

        // position of the mouse
        Vec2 m_position;

        // change in position of the mouse since the last frame
        Vec2 m_positionDelta;

        void onAppUpdate(EVENT_HANDLER(Application));

        void onButtonDown(EVENT_HANDLER(Application));
        void onButtonUp(EVENT_HANDLER(Application));

        void onMove(EVENT_HANDLER(Application));

        void onScroll(EVENT_HANDLER(Application));
    } Meta(Enable);
}