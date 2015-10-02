

#pragma once

#include "Component.h"
#include <Core/Graphics/Core/Camera/Camera.h>
#include <Core/Graphics/API/GFXAPIDefines.h>

class EditorCamera : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    EditorCamera(void);
    ~EditorCamera(void);

    void OnInitialize(void) override;

    // graphics gettors
    GFXHND GetHandle(void) const;
    GFXCamera &GetCamera(void) const;

    // zoom
    const float &GetZoom(void) const;
    void SetZoom(const float zoom);

    // focus position
    const ursine::SVec3 &GetFocusPosition() const;
    void SetFocusPosition(const ursine::SVec3 &focusPosition);

    // render mode
    const ViewportRenderMode GetRenderMode(void) const;
    void SetRenderMode(const ViewportRenderMode renderMode);

    // projection mode
    const GFXCamera::ProjectionMode GetProjectionModev(void) const;
    void SetProjectionMode(const GFXCamera::ProjectionMode projection);

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
    GFXCamera *m_camera;

    // handle to camera object
    GFXHND m_camHandle;
};