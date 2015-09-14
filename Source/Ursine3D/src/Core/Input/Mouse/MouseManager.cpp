#include "UrsinePrecompiled.h"

#include "MouseManager.h"

namespace ursine
{
    CORE_SYSTEM_DEFINITION( MouseManager );

    MouseManager::MouseManager(void)
        : EventDispatcher( this )
        , m_stateLast( 0 )
        , m_stateCurrent( 0 )
    {
        
    }

    MouseManager::~MouseManager(void)
    {
        
    }

    void MouseManager::OnInitialize(void)
    {
        auto *app = Application::Instance;

        app->Connect( APP_UPDATE, this, &MouseManager::onAppUpdate );

        app->GetPlatformEvents( ).Listener(this)
            .On( SDL_MOUSEBUTTONDOWN, &MouseManager::onButtonDown )
            .On( SDL_MOUSEBUTTONUP, &MouseManager::onButtonUp )
            .On( SDL_MOUSEMOTION, &MouseManager::onMove )
            .On( SDL_MOUSEWHEEL, &MouseManager::onScroll );
    }

    void MouseManager::OnRemove(void)
    {
        auto *app = Application::Instance;

        app->Disconnect( APP_UPDATE, this, &MouseManager::onAppUpdate );

        app->GetPlatformEvents( ).Listener(this)
            .Off( SDL_MOUSEBUTTONDOWN, &MouseManager::onButtonDown )
            .Off( SDL_MOUSEBUTTONUP, &MouseManager::onButtonUp )
            .Off( SDL_MOUSEMOTION, &MouseManager::onMove )
            .Off( SDL_MOUSEWHEEL, &MouseManager::onScroll );
    }

    const Vec2 &MouseManager::GetPosition(void) const
    {
        return m_position;
    }

    const Vec2 &MouseManager::GetPositionDelta(void) const
    {
        return m_positionDelta;
    }

    bool MouseManager::IsButtonDown(MouseButton button) const
    {
        return utils::IsFlagSet( m_stateCurrent, SDL_BUTTON(button) );
    }

    bool MouseManager::IsButtonUp(MouseButton button) const
    {
        return !(utils::IsFlagSet( m_stateCurrent, SDL_BUTTON(button) ));
    }

    bool MouseManager::IsButtonTriggeredDown(MouseButton button) const
    {
        return !utils::IsFlagSet( m_stateLast, SDL_BUTTON(button) ) &&
            utils::IsFlagSet( m_stateCurrent, SDL_BUTTON(button) );
    }

    bool MouseManager::IsButtonTriggeredUp(MouseButton button) const
    {
        return !utils::IsFlagSet( m_stateCurrent, SDL_BUTTON(button) ) &&
            utils::IsFlagSet( m_stateLast, SDL_BUTTON(button) );
    }

    void MouseManager::ShowCursor(bool show) const
    {
        SDL_ShowCursor( show );
    }

    void MouseManager::SetWarped(bool warped) const
    {
        SDL_SetRelativeMouseMode( warped ? SDL_TRUE : SDL_FALSE );
    }

    void MouseManager::onAppUpdate(EVENT_HANDLER(Application))
    {
        m_stateLast = m_stateCurrent;

        int x, y;

        m_stateCurrent = SDL_GetMouseState( &x, &y );

        Vec2 position( static_cast<float>( x ), static_cast<float>( y ) );

        m_positionDelta = m_position - position;

        m_position = position;
    }

    void MouseManager::onButtonDown(EVENT_HANDLER(Application))
    {
        EVENT_ATTRS(Application, PlatformEventArgs);

        MouseButtonArgs e( true,
            static_cast<MouseButton>( args->data->button.button ),
            Vec2( static_cast<float>( args->data->button.x ), static_cast<float>( args->data->button.y ) ),
            args->data->button.clicks );

        Dispatch( MM_BUTTON_DOWN, &e );
    }

    void MouseManager::onButtonUp(EVENT_HANDLER(Application))
    {
        EVENT_ATTRS(Application, PlatformEventArgs);

        MouseButtonArgs e( false,
            static_cast<MouseButton>( args->data->button.button ),
            Vec2( static_cast<float>( args->data->button.x ), static_cast<float>( args->data->button.y ) ),
            args->data->button.clicks );

        Dispatch( MM_BUTTON_UP, &e );
    }

    void MouseManager::onMove(EVENT_HANDLER(Application))
    {
        EVENT_ATTRS(Application, PlatformEventArgs);

        MouseMoveArgs e(
            Vec2( static_cast<float>( args->data->motion.x ), static_cast<float>( args->data->motion.y ) ),
            Vec2( static_cast<float>( args->data->motion.xrel ), static_cast<float>( args->data->motion.yrel ) ) );

        Dispatch( MM_MOVE, &e );
    }

    void MouseManager::onScroll(EVENT_HANDLER(Application))
    {
        EVENT_ATTRS(Application, PlatformEventArgs);

        MouseScrollArgs e(
            Vec2( static_cast<float>( args->data->wheel.x ), static_cast<float>( args->data->wheel.y ) ) );

        Dispatch( MM_SCROLL, &e );
    }
}
