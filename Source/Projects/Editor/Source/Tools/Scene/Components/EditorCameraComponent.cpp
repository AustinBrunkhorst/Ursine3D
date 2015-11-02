#include "Precompiled.h"

#include "EditorCameraComponent.h"
#include <Core/Graphics/API/GfxAPI.h>

NATIVE_COMPONENT_DEFINITION( EditorCamera );

EditorCamera::EditorCamera(void)
    : BaseComponent()
                     , m_camZoom( 10.f )
                     , m_camFocus( ursine::SVec3( 0, 0, 0 ) ) {}

EditorCamera::~EditorCamera(void)
{
    GetCoreSystem(ursine::graphics::GfxAPI)->CameraMgr.DestroyCamera( m_camHandle );
}

void EditorCamera::OnInitialize(void)
{
    m_camHandle = GetCoreSystem( ursine::graphics::GfxAPI )->CameraMgr.AddCamera( );

    m_camera = &GetCoreSystem(ursine::graphics::GfxAPI)->CameraMgr.GetCamera( m_camHandle );
}

ursine::graphics::GfxHND EditorCamera::GetHandle(void) const
{
    return m_camHandle;
}

ursine::graphics::Camera &EditorCamera::GetCamera(void) const
{
    return *m_camera;
}

const float &EditorCamera::GetZoom(void) const
{
    return m_camZoom;
}

void EditorCamera::SetZoom(float zoom)
{
    m_camZoom = zoom;
}

const ursine::SVec3 &EditorCamera::GetFocusPosition(void) const
{
    return m_camFocus;
}

void EditorCamera::SetFocusPosition(const ursine::SVec3 &focusPosition)
{
    m_camFocus = focusPosition;
}

const ursine::graphics::ViewportRenderMode EditorCamera::GetRenderMode(void) const
{
    return m_camera->GetRenderMode( );
}

void EditorCamera::SetRenderMode(const ursine::graphics::ViewportRenderMode renderMode)
{
    m_camera->SetRenderMode( renderMode );
}

const ursine::graphics::Camera::ProjectionMode EditorCamera::GetProjectionMode(void) const
{
    return m_camera->GetProjMode( );
}

void EditorCamera::SetProjectionMode(const ursine::graphics::Camera::ProjectionMode projection)
{
    m_camera->SetProjMode( projection );
}

ursine::Vec2 EditorCamera::GetNearFar(void) const
{
    float nearPlane, farPlane;
    m_camera->GetPlanes( nearPlane, farPlane );

    return ursine::Vec2( nearPlane, farPlane );
}

void EditorCamera::SetNearFar(const ursine::Vec2 &nearFar)
{
    m_camera->SetPlanes( nearFar.X( ), nearFar.Y( ) );
}

const float EditorCamera::GetFOV(void) const
{
    return m_camera->GetFOV( );
}

void EditorCamera::SetFOV(const float degrees)
{
    m_camera->SetFOV( degrees );
}

const float EditorCamera::GetOrthoSize(void) const
{
    return m_camera->GetSize( );
}

void EditorCamera::SetOrthoSize(const float size)
{
    m_camera->SetSize( size );
}

const ursine::SVec3 &EditorCamera::GetPosition(void) const
{
    return m_camera->GetPosition( );
}

void EditorCamera::SetPosition(const ursine::SVec3 &position)
{
    m_camera->SetPosition( position );
}

ursine::Vec2 EditorCamera::GetDimensions(void) const
{
    float width, height;
    m_camera->GetDimensions( width, height );

    return ursine::Vec2( width, height );
}

void EditorCamera::SetDimensions(const ursine::Vec2 &dimensions)
{
    m_camera->SetDimensions( dimensions.X( ), dimensions.Y( ) );
}

const ursine::SVec3 &EditorCamera::GetLook(void) const
{
    return m_camera->GetLook( );
}

void EditorCamera::SetLook(const ursine::SVec3 &look)
{
    m_camera->SetLook( look );
}
