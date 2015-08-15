#include "UrsinePrecompiled.h"

#include "MouseManager.h"

namespace Ursine
{
    MouseManager *gMouseManager = nullptr;

    MouseManager::MouseManager(void)
        : EventDispatcher(this)
    {
        gMouseManager = this;

        UAssert(gApplication != nullptr, 
            "Application instance doesn't exist.");

        gApplication->GetInternalEvents().Listener(this)
            .On(SDL_MOUSEBUTTONDOWN, &MouseManager::onButtonDown)
            .On(SDL_MOUSEBUTTONUP, &MouseManager::onButtonUp)
            .On(SDL_MOUSEMOTION, &MouseManager::onMove)
            .On(SDL_MOUSEWHEEL, &MouseManager::onScroll);
    }

    MouseManager::~MouseManager(void)
    {
        gApplication->GetInternalEvents().Listener(this)
            .Off(SDL_MOUSEBUTTONDOWN, &MouseManager::onButtonDown)
            .Off(SDL_MOUSEBUTTONUP, &MouseManager::onButtonUp)
            .Off(SDL_MOUSEMOTION, &MouseManager::onMove)
            .Off(SDL_MOUSEWHEEL, &MouseManager::onScroll);

        gMouseManager = nullptr;
    }

    void MouseManager::Update(void)
    {
        _state_last = _state_current;

        int x, y;

        _state_current = SDL_GetMouseState(&x, &y);

        Vector2 position(static_cast<float>(x), static_cast<float>(y));

        _position_delta = _position - position;

        _position = position;
    }

    const Vector2 &MouseManager::GetPosition(void) const
    {
        return _position;
    }

    const Vector2 &MouseManager::GetPositionDelta(void) const
    {
        return _position_delta;
    }

    bool MouseManager::IsButtonDown(MouseButton button) const
    {
        return Utils::IsFlagSet(_state_current, SDL_BUTTON(button));
    }

    bool MouseManager::IsButtonUp(MouseButton button) const
    {
        return !(Utils::IsFlagSet(_state_current, SDL_BUTTON(button)));
    }

    bool MouseManager::IsButtonTriggeredDown(MouseButton button) const
    {
        return !Utils::IsFlagSet(_state_last, SDL_BUTTON(button)) &&
             Utils::IsFlagSet(_state_current, SDL_BUTTON(button));
    }

    bool MouseManager::IsButtonTriggeredUp(MouseButton button) const
    {
        return !Utils::IsFlagSet(_state_current, SDL_BUTTON(button)) &&
            Utils::IsFlagSet(_state_last, SDL_BUTTON(button));
    }

    void MouseManager::ShowCursor(bool show) const
    {
        SDL_ShowCursor( show );
    }

    void MouseManager::SetWarped(bool warped) const
    {
        SDL_SetRelativeMouseMode( warped ? SDL_TRUE : SDL_FALSE );
    }

    void MouseManager::onButtonDown(EVENT_HANDLER(Application))
    {
        EVENT_ATTRS(Application, InternalApplicationArgs);

        MouseButtonArgs e(true, 
            static_cast<MouseButton>(args->data->button.button), 
            Vector2(static_cast<float>(args->data->button.x), static_cast<float>(args->data->button.y)),
            args->data->button.clicks);

        Dispatch(MM_BUTTON_DOWN, &e);
    }

    void MouseManager::onButtonUp(EVENT_HANDLER(Application))
    {
        EVENT_ATTRS(Application, InternalApplicationArgs);

        MouseButtonArgs e(false,
            static_cast<MouseButton>(args->data->button.button),
            Vector2(static_cast<float>(args->data->button.x), static_cast<float>(args->data->button.y)),
            args->data->button.clicks);

        Dispatch(MM_BUTTON_UP, &e);
    }

    void MouseManager::onMove(EVENT_HANDLER(Application))
    {
        EVENT_ATTRS(Application, InternalApplicationArgs);

        MouseMoveArgs e(
            Vector2(static_cast<float>(args->data->motion.x), static_cast<float>(args->data->motion.y)),
            Vector2(static_cast<float>(args->data->motion.xrel), static_cast<float>(args->data->motion.yrel)));

        Dispatch(MM_MOVE, &e);
    }

    void MouseManager::onScroll(EVENT_HANDLER(Application))
    {
        EVENT_ATTRS(Application, InternalApplicationArgs);

        MouseScrollArgs e(
            Vector2(static_cast<float>(args->data->wheel.x), static_cast<float>(args->data->wheel.y)));

        Dispatch(MM_SCROLL, &e);
    }
}