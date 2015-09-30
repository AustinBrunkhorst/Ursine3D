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
            , m_handle( Application::Instance->GetCoreSystem<GfxAPI>( )->CameraMgr.AddCamera( ) )
        {
            
        }

        Camera::~Camera(void)
        {
            URSINE_TODO( "remove camera handle" );
        }

        GraphicsCamera &Camera::GetCamera(void) const
        {
            return Application::Instance->GetCoreSystem<GfxAPI>( )->CameraMgr.GetCamera( m_handle );
        }
    }
}