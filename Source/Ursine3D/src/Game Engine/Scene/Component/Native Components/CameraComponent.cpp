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
            , m_handle( CoreSystem( GfxAPI )->CameraMgr.AddCamera( ) )
        {
            
        }

        Camera::~Camera(void)
        {
            CoreSystem( GfxAPI )->CameraMgr.DestroyCamera( m_handle );
        }

        GFXCamera &Camera::GetCamera(void) const
        {
            return CoreSystem( GfxAPI )->CameraMgr.GetCamera( m_handle );
        }
    }
}