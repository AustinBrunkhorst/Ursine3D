#include "UrsinePrecompiled.h"

#include "CameraComponent.h"

#include "GfxAPI.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( Camera );

        Camera::Camera(void)
            : BaseComponent( )
            , m_handle( GetCoreSystem(graphics::GfxAPI )->CameraMgr.AddCamera( ) )
        {
        }

        Camera::~Camera(void)
        {
            GetCoreSystem( graphics::GfxAPI )->CameraMgr.DestroyCamera( m_handle );
        }

        void Camera::OnInitialize()
        {
            GetCoreSystem( graphics::GfxAPI )->CameraMgr.GetCamera( m_handle ).SetEntityID( GetOwner( )->GetID( ) );
        }

        graphics::GfxHND Camera::GetHandle(void) const
        {
            return m_handle;
        }

        graphics::Camera *Camera::GetCamera(void) const
        {
            return &GetCoreSystem( graphics::GfxAPI )->CameraMgr.GetCamera( m_handle );
        }
    }
}