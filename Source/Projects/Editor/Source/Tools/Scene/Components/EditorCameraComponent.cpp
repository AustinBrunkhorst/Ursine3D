#include "Precompiled.h"

#include "EditorCameraComponent.h"
#include <Core/Graphics/API/GfxAPI.h>

NATIVE_COMPONENT_DEFINITION(EditorCamera);

EditorCamera::EditorCamera()
    : BaseComponent()
    , m_camZoom(10.f)
    , m_camFocus(ursine::SVec3(0, 0, 0))
{
}

EditorCamera::~EditorCamera()
{
    GetCoreSystem(ursine::GfxAPI)->CameraMgr.DestroyCamera(m_camHandle);
}

void EditorCamera::OnInitialize()
{
    m_camHandle = GetCoreSystem( ursine::GfxAPI )->CameraMgr.AddCamera();

    m_camera = &GetCoreSystem(ursine::GfxAPI)->CameraMgr.GetCamera(m_camHandle);
}

GFXHND EditorCamera::GetHandle() const
{
    return m_camHandle;
}

GFXCamera& EditorCamera::GetCamera() const
{
    return *m_camera;
}

const float &EditorCamera::GetZoom() const
{
    return m_camZoom;
}

void EditorCamera::SetZoom(float zoom)
{
    m_camZoom = zoom;
}

const ursine::SVec3& EditorCamera::GetFocusPosition() const
{
    return m_camFocus;
}

void EditorCamera::SetFocusPosition(const ursine::SVec3& focusPosition)
{
    m_camFocus = focusPosition;
}

const ViewportRenderMode EditorCamera::GetRenderMode() const
{
    return m_camera->GetRenderMode();
}

void EditorCamera::SetRenderMode(const ViewportRenderMode renderMode)
{
    m_camera->SetRenderMode(renderMode);
}

const GFXCamera::ProjectionMode EditorCamera::GetProjectionModev() const
{
    return m_camera->GetProjMode();
}

void EditorCamera::SetProjectionMode(const GFXCamera::ProjectionMode projection)
{
    m_camera->SetProjMode(projection);
}

ursine::Vec2 EditorCamera::GetNearFar() const
{
    float nearPlane, farPlane;
    m_camera->GetPlanes(nearPlane, farPlane);

    return ursine::Vec2(nearPlane, farPlane);
}

void EditorCamera::SetNearFar(const ursine::Vec2& nearFar)
{
    m_camera->SetPlanes(nearFar.X(), nearFar.Y());
}

const float EditorCamera::GetFOV() const
{
    return m_camera->GetFOV();
}

void EditorCamera::SetFOV(const float degrees)
{
    m_camera->SetFOV(degrees);
}

const float EditorCamera::GetOrthoSize() const
{
    return m_camera->GetSize();
}

void EditorCamera::SetOrthoSize(const float size)
{
    m_camera->SetSize(size);
}

const ursine::SVec3& EditorCamera::GetPosition() const
{
    return m_camera->GetPosition();
}

void EditorCamera::SetPosition(const ursine::SVec3& position)
{
    m_camera->SetPosition(position);
}

ursine::Vec2 EditorCamera::GetDimensions() const
{
    float width, height;
    m_camera->GetDimensions(width, height);

    return ursine::Vec2(width, height);
}

void EditorCamera::SetDimensions(const ursine::Vec2& dimensions)
{
    m_camera->SetDimensions(dimensions.X(), dimensions.Y());
}

const ursine::SVec3& EditorCamera::GetLook() const
{
    return m_camera->GetLook();
}

void EditorCamera::SetLook(const ursine::SVec3& look)
{
    m_camera->SetLook(look);
}


