#pragma once

#include <EntitySystem.h>
#include <Core/Graphics/API/GfxAPI.h>
#include <Editor.h>

class EditorCameraSystem : public ursine::ecs::EntitySystem
{
    ENTITY_SYSTEM;

    //public methods
public:
    EditorCameraSystem(ursine::ecs::World *world);
    void SetEditorCamera(ursine::graphics::Camera *camera);
    ursine::graphics::Camera *GetEditorCamera();

    //private methods
private:
    void OnInitialize(void) override;
    void OnRemove(void) override;

    //events
    void onUpdate(EVENT_HANDLER(ursine::ecs::World));
    void onMouseScroll(EVENT_HANDLER(ursine::MouseManager));

    //updating camera methods
    void updateCameraKeys(float dt);
    void updateCameraMouse(float dt);

    //private members
private:
    ursine::graphics::Camera *m_camera;
    float m_camZoom;
    ursine::SVec3 m_camPos;
    Editor *m_editor;

    ursine::TweenID m_cameraTransition;
    

} Meta(Enable);