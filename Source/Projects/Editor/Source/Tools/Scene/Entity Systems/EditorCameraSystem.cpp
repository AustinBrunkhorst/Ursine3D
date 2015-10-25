#include "Precompiled.h"

#include "EditorCameraSystem.h"
#include <Core/Input/Mouse/MouseManager.h>
#include <Core/Input/Keyboard/KeyboardManager.h>
#include <SystemManager.h>

#include "ObjectSelectorSystem.h"

ENTITY_SYSTEM_DEFINITION(EditorCameraSystem);

EditorCameraSystem::EditorCameraSystem(ursine::ecs::World* world)
    : EntitySystem(world)
    , m_camera(nullptr)
    , m_camZoom(5.0f)
    , m_camPos(ursine::SVec3(0, 0, 0))
    , m_editor(GetCoreSystem(Editor))
{   
}

void EditorCameraSystem::SetEditorCamera(ursine::graphics::Camera* camera)
{
    m_camera = camera;
}

void EditorCameraSystem::OnInitialize()
{
    GetCoreSystem(ursine::MouseManager)->Listener(this)
        .On( ursine::MM_SCROLL, &EditorCameraSystem::onMouseScroll);

    m_world->Listener(this)
        .On(ursine::ecs::WorldEventType::WORLD_UPDATE, &EditorCameraSystem::onUpdate);

    m_cameraTransition = m_tweens.Create( );
}

ursine::graphics::Camera *EditorCameraSystem::GetEditorCamera()
{
    return m_camera;
}

void EditorCameraSystem::OnRemove()
{
    GetCoreSystem(ursine::MouseManager)->Listener(this)
        .Off(ursine::MM_SCROLL, &EditorCameraSystem::onMouseScroll);

    m_world->Listener(this)
        .Off(ursine::ecs::WorldEventType::WORLD_UPDATE, &EditorCameraSystem::onUpdate);
}

void EditorCameraSystem::onUpdate(EVENT_HANDLER(ursine::ecs::World))
{
    using namespace ursine;

    EVENT_ATTRS(Application, EventArgs);

    auto app = Application::Instance;
    auto dt = app->GetDeltaTime();

    auto *keyboardMgr = GetCoreSystem(KeyboardManager);

    // first, update all of the camera stuff. 
    //keyboard controls
    updateCameraKeys(dt);

    // mouse controls
    if (keyboardMgr->GetModifiers() & KMD_ALT)
    {
        updateCameraMouse(dt);
    }

    //our position always needs to be relative to the center position
    graphics::Camera &cam = *m_camera;
    SVec3 look = cam.GetLook();

    //normalize look and scale by zoom
    look.Normalize();
    look = look * m_camZoom;

    //negate the vector, opposite of look is going away from center
    cam.SetPosition(m_camPos - look);
}

void EditorCameraSystem::onMouseScroll(EVENT_HANDLER(ursine::MouseManager))
{
    EVENT_ATTRS(ursine::MouseManager, ursine::MouseScrollArgs);


    m_camZoom -= args->delta.Y();

    if (m_camZoom < 1)
        m_camZoom = 1.f;
}

void EditorCameraSystem::updateCameraKeys(float dt)
{
    auto *keyboardMgr = GetCoreSystem(ursine::KeyboardManager);

    float speed = 3;

    //focus with f
    if(GetCoreSystem(ursine::KeyboardManager)->IsTriggeredDown( ursine::KeyboardKey::KEY_F))
    {
        if (m_world->GetEntitySystem(ObjectSelectorSystem)->
            GetCurrentFocus() != nullptr)
        {
            //get target position
            ursine::SVec3 target = m_world->GetEntitySystem(ObjectSelectorSystem)->
                GetCurrentFocus()->GetComponent<ursine::ecs::Transform>()->GetWorldPosition();

            float targetZoom = m_world->GetEntitySystem(ObjectSelectorSystem)->
                GetCurrentFocus()->GetComponent<ursine::ecs::Transform>()->GetWorldScale().Length() * 2;

            auto duration = ursine::TimeSpan::FromSeconds( 0.75f );
            auto ease = ursine::ease::QuarticOut;

            m_cameraTransition.Stop()
                .BeginGroup()
                    .Property(m_camPos, target, duration, ease )
                    .Property(m_camZoom, targetZoom, duration, ease )
                .EndGroup();
        }
    }

    //get the camera
    ursine::graphics::Camera &cam = *m_camera;
    ursine::SVec3 look = cam.GetLook();

    ///////////////////////////////////////////////////////////////////
    // KEYBOARD MOVEMENT //////////////////////////////////////////////
    //get other camera data, now that the look was set
    auto right = cam.GetRight();
    auto pos = m_camPos;
    auto up = cam.GetUp();
    auto dir = ursine::SVec3(0, 0, 0);

    //update position
    if (keyboardMgr->IsDown( ursine::KEY_W))
    {
        dir += look;
    }
    if (keyboardMgr->IsDown( ursine::KEY_S))
    {
        dir -= look;
    }
    if (keyboardMgr->IsDown( ursine::KEY_A))
    {
        dir += right;
    }
    if (keyboardMgr->IsDown( ursine::KEY_D))
    {
        dir -= right;
    }

    if (keyboardMgr->IsDown( ursine::KEY_E))
    {
        dir += up;
    }
    if (keyboardMgr->IsDown( ursine::KEY_Q))
    {
        dir -= up;
    }

    //make sure something happened
    if (dir.Length() > 0)
    {
        //normalize vector, scale by dt and speed
        dir.Normalize();
        dir *= dt * speed;

        //apply to position
        pos += dir;

        //set new pos
        m_camPos = pos;
    }
}

void EditorCameraSystem::updateCameraMouse(float dt)
{
    auto *mouseMgr = GetCoreSystem(ursine::MouseManager);

    //get the camera
    ursine::graphics::Camera &cam = *m_camera;

    ursine::SVec3 look = cam.GetLook();
    ursine::SVec3 up = cam.GetUp();
    ursine::SVec3 right = cam.GetRight();

    ///////////////////////////////////////////////////////////////////
    // CAMERA ROTATION
    if (mouseMgr->IsButtonDown( ursine::MBTN_LEFT))
    {
        auto mouseDelta = mouseMgr->GetPositionDelta();
        auto camTransform = cam.GetViewMatrix();

        //we need to limit the up delta so that we can't wrap if we are at the very top/bottom
        mouseDelta /= 2.f;

        if (mouseDelta.Length() > 0)
        {
            //generate the up rotation
            auto upRotation = ursine::SQuat(-mouseDelta.Y(), camTransform.TransformVector( ursine::SVec3(1.0, 0.0, 0.0)));

            //generate side rotation
            auto sideRotation = ursine::SQuat(-mouseDelta.X(), ursine::SVec3(0, 1, 0));

            //transform w/ rotations
            look = sideRotation.Rotate(look);
            look = upRotation.Rotate(look);

            cam.SetLook(look);
        }
    }

    ///////////////////////////////////////////////////////////////////
    // PANNING
    else if (mouseMgr->IsButtonDown( ursine::MBTN_MIDDLE))
    {
        auto mouseDelta = mouseMgr->GetPositionDelta();

        if (mouseDelta.Length() > 0)
        {
            float width, height;

            cam.GetDimensions(width, height);

            m_camPos += right * -mouseDelta.X() * dt * width;

            m_camPos += up * -mouseDelta.Y() * dt * height;
        }
    }

    ///////////////////////////////////////////////////////////////////
    // ZOOM
    else if (mouseMgr->IsButtonDown( ursine::MBTN_RIGHT))
    {
        auto mouseDelta = mouseMgr->GetPositionDelta();

        if (mouseDelta.Length() > 0)
        {
            m_camZoom += -mouseDelta.Y() * dt;

            if (m_camZoom < 1)
            {
                m_camZoom = 1;
            }
        }
    }

    //if in here, draw the center mark
    float halfSize = 0.3f;

    auto gfx = GetCoreSystem(ursine::graphics::GfxAPI);

    using namespace ursine;

    gfx->DrawingMgr.SetColor(1, 0, 0, 1);
    gfx->DrawingMgr.DrawLine(m_camPos - SVec3(halfSize, 0, 0), m_camPos + SVec3(halfSize, 0, 0));
    gfx->DrawingMgr.DrawPoint(m_camPos + SVec3(halfSize, 0, 0));
    gfx->DrawingMgr.SetColor(0, 1, 0, 1);
    gfx->DrawingMgr.DrawLine(m_camPos - SVec3(0, halfSize, 0), m_camPos + SVec3(0, halfSize, 0));
    gfx->DrawingMgr.DrawPoint(m_camPos + SVec3(0, halfSize, 0));
    gfx->DrawingMgr.SetColor(0, 0, 1, 1);
    gfx->DrawingMgr.DrawLine(m_camPos - SVec3(0, 0, halfSize), m_camPos + SVec3(0, 0, halfSize));
    gfx->DrawingMgr.DrawPoint(m_camPos + SVec3(0, 0, halfSize));

}

ursine::SVec3 EditorCameraSystem::GetEditorFocusPosition()
{
    return m_camPos;
}

float EditorCameraSystem::GetCamZoom()
{
    return m_camZoom;
}