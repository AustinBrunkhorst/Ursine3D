#pragma once

#include <Component.h>
#include <Core/Graphics/Core/Camera/Camera.h>
#include <Core/Graphics/API/GFXAPIDefines.h>

class EditorCamera : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    Meta(Enable)
    EditorCamera(void);
    ~EditorCamera(void);

    void OnInitialize(void) override;

    // graphics gettors
    ursine::graphics::GfxHND GetHandle(void) const;
    ursine::graphics::Camera &GetCamera(void) const;

    // zoom
    const float &GetZoom(void) const;
    void SetZoom(const float zoom);

    // focus position
    const ursine::SVec3 &GetFocusPosition() const;
    void SetFocusPosition(const ursine::SVec3 &focusPosition);

    // render mode
    const ursine::graphics::ViewportRenderMode GetRenderMode(void) const;
    void SetRenderMode(const ursine::graphics::ViewportRenderMode renderMode);

    // projection mode
    const ursine::graphics::Camera::ProjectionMode GetProjectionMode(void) const;
    void SetProjectionMode(const ursine::graphics::Camera::ProjectionMode projection);

    // near/far
    ursine::Vec2 GetNearFar(void) const;
    void SetNearFar(const ursine::Vec2& nearFar);

    // fov
    const float GetFOV(void) const;
    void SetFOV(float degrees);

    // ortho size
    const float GetOrthoSize(void) const;
    void SetOrthoSize(float size);

    // actual position
    const ursine::SVec3 &GetPosition(void) const;
    void SetPosition(const ursine::SVec3& position);

    // width/height
    ursine::Vec2 GetDimensions(void) const;
    void SetDimensions(const ursine::Vec2 &dimensions);

    // look
    const ursine::SVec3 &GetLook(void) const;
    void SetLook(const ursine::SVec3 &look);
    
private:
    // zoom amount
    float m_camZoom;

    // position of focus
    ursine::SVec3 m_camFocus;

    // camera ptr
    ursine::graphics::Camera *m_camera;

    // handle to camera object
    ursine::graphics::GfxHND m_camHandle;
};