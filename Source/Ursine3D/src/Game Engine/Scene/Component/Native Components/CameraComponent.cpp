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
            , m_handle( GetCoreSystem( GfxAPI )->CameraMgr.AddCamera( ) )
        {
            
        }

        Camera::~Camera(void)
        {
            GetCoreSystem( GfxAPI )->CameraMgr.DestroyCamera( m_handle );
        }

        GFXHND Camera::GetHandle(void) const
        {
            return m_handle;
        }

        GFXCamera &Camera::GetCamera(void) const
        {
            return GetCoreSystem( GfxAPI )->CameraMgr.GetCamera( m_handle );
        }
    }
}