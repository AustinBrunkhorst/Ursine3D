/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EditorCameraSystem.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "EditorCameraSystem.h"
#include "EditorToolSystem.h"

#include <CameraComponent.h>

#include <MouseManager.h>
#include <KeyboardManager.h>
#include <SystemManager.h>

using namespace ursine;

ENTITY_SYSTEM_DEFINITION( EditorCameraSystem );

namespace
{
    const auto kEditorCameraEntityName = "Editor Camera";
}

EditorCameraSystem::EditorCameraSystem(ecs::World *world)
    : EntitySystem( world )
    , m_hasFocus( false )
    , m_hasMouseFocus( false )
    , m_cameraEntity( nullptr )
    , m_camera( nullptr )
    , m_camZoom( 50.0f )
    , m_camPos( SVec3( 0, 0, 0 ) ) { }

bool EditorCameraSystem::HasFocus(void) const
{
    return m_hasFocus;
}

void EditorCameraSystem::SetFocus(bool focus)
{
    m_hasFocus = focus;
}

bool EditorCameraSystem::HasMouseFocus(void) const
{
    return m_hasMouseFocus;
}

void EditorCameraSystem::SetMouseFocus(bool focus)
{
    m_hasMouseFocus = focus;
}

ecs::Camera *EditorCameraSystem::GetEditorCamera(void)
{
    if (!m_cameraEntity)
        return nullptr;

    return m_cameraEntity->GetComponent<ecs::Camera>( );
}

ursine::ecs::Entity* EditorCameraSystem::GetEditorCameraEntity(void)
{
    return m_cameraEntity;
}

void EditorCameraSystem::OnInitialize(void)
{
    m_focusTransition = m_tweens.Create( );

    GetCoreSystem( MouseManager )->Listener( this )
        .On( MM_SCROLL, &EditorCameraSystem::onMouseScroll );

    m_world->Listener( this )
        .On( ecs::WorldEventType::WORLD_EDITOR_UPDATE, &EditorCameraSystem::onUpdate );
}

void EditorCameraSystem::OnAfterLoad(void)
{
    auto *oldCamera = m_world->GetEntityFromName( kEditorCameraEntityName );

    if (oldCamera)
        oldCamera->Delete( );

    m_cameraEntity = m_world->CreateEntity( kEditorCameraEntityName );

    m_cameraEntity->EnableDeletion( false );
	m_cameraEntity->EnableSerialization( false );

    m_camera = m_cameraEntity->AddComponent<ecs::Camera>( );

    m_camera->SetEditorCamera( true );
    m_camera->SetViewportPosition( Vec2::Zero( ) );
    m_cameraEntity->GetTransform( )->SetWorldPosition( Vec3( -50, 50, -50 ) );
    m_camera->SetViewportSize( Vec2::One( ) );
    m_camera->SetNearPlane( 0.1f );
    m_camera->SetFarPlane( 2500.0f );
    m_camera->SetFOV( 45.f );

    m_camZoom = SVec3( -50, 50, 50 ).Length( );
    m_cameraEntity->GetTransform( )->LookAt( { 0.0f, 0.0f, 0.0f } );
}

void EditorCameraSystem::OnRemove(void)
{
    auto *mm = GetCoreSystem( MouseManager );

    GetCoreSystem( MouseManager )->Listener( this )
        .Off( MM_SCROLL, &EditorCameraSystem::onMouseScroll );

    m_world->Listener( this )
        .Off( ecs::WorldEventType::WORLD_EDITOR_UPDATE, &EditorCameraSystem::onUpdate );
}

void EditorCameraSystem::onUpdate(EVENT_HANDLER(ecs::World))
{
    EVENT_ATTRS(ecs::World, EventArgs);

    // don't update on focus
    if (!m_hasFocus)
        return;

    auto dt = Application::Instance->GetDeltaTime( );

    auto *keyboardMgr = GetCoreSystem( KeyboardManager );

    // first, update all of the camera stuff. 
    // keyboard controls
    updateCameraKeys( dt );

    // mouse controls
    if (keyboardMgr->GetModifiers( ) & KMD_ALT)
    {
        updateCameraMouse( dt );
    }

    // our position always needs to be relative to the center position
    auto look = m_cameraEntity->GetTransform( )->GetForward( );

    // normalize look and scale by zoom
    look.Normalize( );
    look = look * m_camZoom;

    // negate the vector, opposite of look is going away from center
    m_cameraEntity->GetTransform( )->SetWorldPosition( m_camPos - look );
}

void EditorCameraSystem::onMouseScroll(EVENT_HANDLER(MouseManager))
{
    EVENT_ATTRS(MouseManager, MouseScrollArgs);

    // don't update on focus
    if (!m_hasFocus)
        return;

    m_camZoom -= args->delta.Y( ) * 15.0f;

    if (m_camZoom < 1)
        m_camZoom = 1.0f;
}

void EditorCameraSystem::updateCameraKeys(float dt)
{
    auto *keyboardMgr = GetCoreSystem(KeyboardManager);

    float speed = 15;

    // focus with f
    if (keyboardMgr->IsTriggeredDown( KEY_F ))
    {
        auto selectorSystem = m_world->GetEntitySystem( EditorToolSystem );
        auto currentFocus = selectorSystem->GetCurrentFocus( );

        if (currentFocus != nullptr)
        {
            auto transform = currentFocus->GetTransform( );

            // get target position
            SVec3 target = currentFocus->GetTransform( )->GetWorldPosition( );

            float targetZoom = transform->GetWorldScale( ).Length( ) * 2.0f;

            auto duration = TimeSpan::FromSeconds( 0.75f );
            auto ease = ease::QuarticOut;

            m_focusTransition.Stop( )
                .BeginGroup( )
                    .Property( m_camPos, target, duration, ease )
                    .Property( m_camZoom, targetZoom, duration, ease )
                .EndGroup( );
        }
    }

    // get the camera
    SVec3 look = m_camera->GetLook( );

    ///////////////////////////////////////////////////////////////////
    // KEYBOARD MOVEMENT //////////////////////////////////////////////
    // get other camera data, now that the look was set
    auto right = m_camera->GetRight( );
    auto pos = m_camPos;
    auto up = m_camera->GetUp( );
    auto dir = SVec3( 0, 0, 0 );

    auto modifiers = keyboardMgr->GetModifiers( );

    // don't move if CTRL is down
    if (!utils::IsFlagSet( modifiers, KMD_LCTRL ) && 
        !utils::IsFlagSet( modifiers, KMD_RCTRL ))
    {
        if (keyboardMgr->IsDown( KEY_W ))
        {
            dir += look;
        }

        if (keyboardMgr->IsDown( KEY_S ))
        {
            dir -= look;
        }

        if (keyboardMgr->IsDown( KEY_A ))
        {
            dir -= right;
        }

        if (keyboardMgr->IsDown( KEY_D ))
        {
            dir += right;
        }

        if (keyboardMgr->IsDown( KEY_E ))
        {
            dir += up;
        }

        if (keyboardMgr->IsDown( KEY_Q ))
        {
            dir -= up;
        }
    }

    // make sure something happened
    if (dir.Length( ) > 0)
    {
        // normalize vector, scale by dt and speed
        dir.Normalize( );
        dir *= dt * speed;

        // apply to position
        pos += dir;

        // set new pos
        m_camPos = pos;
    }
}

void EditorCameraSystem::updateCameraMouse(float dt)
{
    auto *mouseMgr = GetCoreSystem( MouseManager );

    //get the camera
    auto &cam = *m_camera;

    auto look = cam.GetLook( );
    auto up = cam.GetUp( );
    auto right = cam.GetRight( );


    ///////////////////////////////////////////////////////////////////
    // CAMERA ROTATION
    if (mouseMgr->IsButtonDown( MBTN_LEFT ))
    {
        auto mouseDelta = mouseMgr->GetPositionDelta( );
        auto camTransform = cam.GetViewMatrix( );

        if (mouseDelta.Length( ) > 0)
        {
            //generate the up rotation
            auto upRotation = SQuat( -mouseDelta.Y( ), camTransform.TransformVector( SVec3( 1.0, 0.0, 0.0 ) ) );

            //generate side rotation
            auto sideRotation = SQuat( -mouseDelta.X( ), SVec3( 0, 1, 0 ) );

            //transform w/ rotations
            look = sideRotation.Rotate( look );
            look = upRotation.Rotate( look );

            cam.SetLook( m_camPos + look * m_camZoom );
        }
    }

    ///////////////////////////////////////////////////////////////////
    // PANNING
    else if (mouseMgr->IsButtonDown( MBTN_MIDDLE ))
    {
        auto mouseDelta = mouseMgr->GetPositionDelta( );

        if (mouseDelta.Length( ) > 0)
        {
            auto size = cam.GetViewportSize( );

            m_camPos += right * mouseDelta.X( ) * dt * size.X( ) * 2;

            m_camPos -= up * mouseDelta.Y( ) * dt * size.Y( ) * 2;
        }
    }

    ///////////////////////////////////////////////////////////////////
    // ZOOM
    else if (mouseMgr->IsButtonDown( MBTN_RIGHT ))
    {
        auto mouseDelta = mouseMgr->GetPositionDelta( );

        if (mouseDelta.Length( ) > 0)
        {
            m_camZoom += -mouseDelta.Y( ) * 15.0f * dt;

            if (m_camZoom < 1)
            {
                m_camZoom = 1;
            }
        }
    }


    // if in here, draw the center mark
    float halfSize = 0.3f;

    auto &drawingManager = GetCoreSystem( graphics::GfxAPI )->DrawingMgr;

    drawingManager.SetColor( 1, 0, 0, 1 );
    drawingManager.DrawLine( m_camPos - SVec3( halfSize, 0, 0 ), m_camPos + SVec3( halfSize, 0, 0 ) );
    drawingManager.DrawPoint( m_camPos + SVec3( halfSize, 0, 0 ) );
    drawingManager.SetColor( 0, 1, 0, 1 );
    drawingManager.DrawLine( m_camPos - SVec3( 0, halfSize, 0 ), m_camPos + SVec3( 0, halfSize, 0 ) );
    drawingManager.DrawPoint( m_camPos + SVec3( 0, halfSize, 0 ) );
    drawingManager.SetColor( 0, 0, 1, 1 );
    drawingManager.DrawLine( m_camPos - SVec3( 0, 0, halfSize ), m_camPos + SVec3( 0, 0, halfSize ) );
    drawingManager.DrawPoint( m_camPos + SVec3( 0, 0, halfSize ) );
}

SVec3 EditorCameraSystem::GetEditorFocusPosition(void)
{
    return m_camPos;
}

float EditorCameraSystem::GetCamZoom(void)
{
    return m_camZoom;
}
