#pragma once

#include <EntitySystem.h>

#include <GfxAPI.h>

class EditorCameraSystem : public ursine::ecs::EntitySystem
{
    ENTITY_SYSTEM;

public:
    EditorCameraSystem(ursine::ecs::World *world);

    // Determines if the scene representing this world currently has focus
    bool HasFocus(void) const;

    void SetFocus(bool focus);

    // Determines if the scene representing this world currently has mouse focus
    // i.e. the mouse is inside the scene viewport
    bool HasMouseFocus(void) const;
    void SetMouseFocus(bool focus);

    ursine::graphics::Camera *GetEditorCamera(void);

    ursine::SVec3 GetEditorFocusPosition(void);
    float GetCamZoom(void);

private:
    bool m_hasFocus;
    bool m_hasMouseFocus;

    ursine::ecs::Entity *m_cameraEntity;

    ursine::graphics::Camera *m_camera;
    float m_camZoom;
    ursine::SVec3 m_camPos;

    ursine::TweenID m_focusTransition;

    void OnInitialize(void) override;
    void OnRemove(void) override;

    // events
    void onUpdate(EVENT_HANDLER(ursine::ecs::World));
    void onMouseScroll(EVENT_HANDLER(ursine::MouseManager));

    // updating camera methods
    void updateCameraKeys(float dt);
    void updateCameraMouse(float dt);

} Meta(Enable);