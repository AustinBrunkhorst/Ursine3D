#include "Precompiled.h"

#include "SceneView.h"

#include "Project.h"

#include <MouseManager.h>
#include <KeyboardManager.h>

using namespace ursine;

SceneView::SceneView(JSHandlerArgs)
    : m_editor( GetCoreSystem( Editor ) )
      , m_camZoom( 5.0f )
{
    Listener( this )
        .On( "viewportInvalidated", &SceneView::onViewportInvalidated );

    GetCoreSystem( MouseManager )->Listener( this )
        .On( MM_SCROLL, &SceneView::onMouseScroll );

    Application::Instance->Listener( this )
        .On( APP_UPDATE, &SceneView::onAppUpdate );
}

SceneView::~SceneView(void)
{
    Listener( this )
        .Off( "viewportInvalidated", &SceneView::onViewportInvalidated );

    GetCoreSystem( MouseManager )->Listener( this )
        .Off( MM_SCROLL, &SceneView::onMouseScroll );

    Application::Instance->Listener( this )
        .Off( APP_UPDATE, &SceneView::onAppUpdate );
}

GFXCamera &SceneView::getEditorCamera(void)
{
    return GetCoreSystem( GfxAPI )->CameraMgr.GetCamera(
        m_editor->GetProject( )->GetScene( ).GetEditorCamera( )
    );
}

void SceneView::onAppUpdate(EVENT_HANDLER(ursine::Application))
{
    EVENT_ATTRS( Application, EventArgs );

    if (!IsFocused( ))
        return;

    auto app = Application::Instance;
    auto dt = app->GetDeltaTime( );

    auto *keyboardMgr = GetCoreSystem(KeyboardManager);

    // first, update all of the camera stuff. 
    //keyboard controls
    UpdateCameraKeys( dt );

    // mouse controls
    if (keyboardMgr->GetModifiers( ) & KMD_ALT)
    {
        UpdateCameraMouse( dt );
    }

    //our position always needs to be relative to the center position
    GFXCamera &cam = getEditorCamera( );
    SVec3 look = cam.GetLook( );

    //normalize look and scale by zoom
    look.Normalize( );
    look = look * m_camZoom;

    //negate the vector, opposite of look is going away from center
    cam.SetPosition( m_camPos - look );
}

void SceneView::onViewportInvalidated(EVENT_HANDLER(NativeEditorTool))
{
    EVENT_ATTRS(NativeEditorTool, NativeToolEvent);

    auto &scene = m_editor->GetProject( )->GetScene( );

    auto x = static_cast<unsigned>(
        args->data->GetValue( "x" )->GetDoubleValue( )
    );

    auto y = static_cast<unsigned>(
        args->data->GetValue( "y" )->GetDoubleValue( )
    );

    auto width = math::Max( 1u, static_cast<unsigned>(
            args->data->GetValue( "width" )->GetDoubleValue( )
        )
    );

    auto height = math::Max( 1u, static_cast<unsigned>(
            args->data->GetValue( "height" )->GetDoubleValue( )
        )
    );

    auto handle = scene.GetViewport( );

    auto &viewport =
            GetCoreSystem( GfxAPI )->ViewportMgr.GetViewport( handle );

    unsigned oldX, oldY;

    viewport.GetPosition( oldX, oldY );

    if (x != oldX || y != oldY)
    {
        viewport.SetPosition( x, y );
    }

    unsigned oldWidth, oldHeight;

    viewport.GetDimensions( oldWidth, oldHeight );

    if (width != oldWidth || height != oldHeight)
    {
        viewport.SetDimensions( width, height );
    }
}

void SceneView::onMouseScroll(EVENT_HANDLER(MouseManager))
{
    EVENT_ATTRS(MouseManager, MouseScrollArgs);

    if (!m_hasCursorFocus)
        return;

    m_camZoom -= args->delta.Y( );

    if (m_camZoom < 1)
        m_camZoom = 1.f;
}

void SceneView::UpdateCameraKeys(float dt)
{
    auto *keyboardMgr = GetCoreSystem(KeyboardManager);

    float speed = 3;

    //get the camera
    GFXCamera &cam = getEditorCamera( );
    SVec3 look = cam.GetLook( );

    ///////////////////////////////////////////////////////////////////
    // KEYBOARD MOVEMENT //////////////////////////////////////////////
    //get other camera data, now that the look was set
    auto right = cam.GetRight( );
    auto pos = m_camPos;
    auto up = cam.GetUp( );
    auto dir = SVec3( 0, 0, 0 );

    //update position
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
        dir += right;
    }
    if (keyboardMgr->IsDown( KEY_D ))
    {
        dir -= right;
    }

    if (keyboardMgr->IsDown( KEY_E ))
    {
        dir += up;
    }
    if (keyboardMgr->IsDown( KEY_Q ))
    {
        dir -= up;
    }

    //make sure something happened
    if (dir.Length( ) > 0)
    {
        //normalize vector, scale by dt and speed
        dir.Normalize( );
        dir *= dt * speed;

        //apply to position
        pos += dir;

        //set new pos
        m_camPos = pos;
    }
}

void SceneView::UpdateCameraMouse(float dt)
{
    auto *mouseMgr = GetCoreSystem(MouseManager);

    //get the camera
    GFXCamera &cam = getEditorCamera( );

    SVec3 look = cam.GetLook( );
    SVec3 up = cam.GetUp( );
    SVec3 right = cam.GetRight( );

    ///////////////////////////////////////////////////////////////////
    // CAMERA ROTATION
    if (mouseMgr->IsButtonDown( MBTN_LEFT ))
    {
        auto mouseDelta = mouseMgr->GetPositionDelta( );
        auto camTransform = cam.GetViewMatrix( );

        //we need to limit the up delta so that we can't wrap if we are at the very top/bottom
        mouseDelta /= 2.f;

        if (mouseDelta.Length( ) > 0)
        {
            //generate the up rotation
            auto upRotation = SQuat( -mouseDelta.Y( ), camTransform.TransformVector( SVec3( 1.0, 0.0, 0.0 ) ) );

            //generate side rotation
            auto sideRotation = SQuat( -mouseDelta.X( ), SVec3( 0, 1, 0 ) );

            //transform w/ rotations
            look = sideRotation.Rotate( look );
            look = upRotation.Rotate( look );

            cam.SetLook( look );
        }
    }

    ///////////////////////////////////////////////////////////////////
    // PANNING
    else if (mouseMgr->IsButtonDown( MBTN_MIDDLE ))
    {
        auto mouseDelta = mouseMgr->GetPositionDelta( );

        if (mouseDelta.Length( ) > 0)
        {
            float width, height;

            cam.GetDimensions( width, height );

            m_camPos += right * -mouseDelta.X( ) * dt * width;

            m_camPos += up * -mouseDelta.Y( ) * dt * height;
        }
    }

    ///////////////////////////////////////////////////////////////////
    // ZOOM
    else if (mouseMgr->IsButtonDown( MBTN_RIGHT ))
    {
        auto mouseDelta = mouseMgr->GetPositionDelta( );

        if (mouseDelta.Length( ) > 0)
        {
            m_camZoom += -mouseDelta.Y( ) * dt;

            if (m_camZoom < 1)
            {
                m_camZoom = 1;
            }
        }
    }

    //if in here, draw the center mark
    float halfSize = 0.3f;

    auto gfx = GetCoreSystem( GfxAPI );

    gfx->DrawingMgr.SetColor( 1, 0, 0, 1 );
    gfx->DrawingMgr.DrawLine( m_camPos - SVec3( halfSize, 0, 0 ), m_camPos + SVec3( halfSize, 0, 0 ) );
    gfx->DrawingMgr.DrawPoint( m_camPos + SVec3( halfSize, 0, 0 ) );
    gfx->DrawingMgr.SetColor( 0, 1, 0, 1 );
    gfx->DrawingMgr.DrawLine( m_camPos - SVec3( 0, halfSize, 0 ), m_camPos + SVec3( 0, halfSize, 0 ) );
    gfx->DrawingMgr.DrawPoint( m_camPos + SVec3( 0, halfSize, 0 ) );
    gfx->DrawingMgr.SetColor( 0, 0, 1, 1 );
    gfx->DrawingMgr.DrawLine( m_camPos - SVec3( 0, 0, halfSize ), m_camPos + SVec3( 0, 0, halfSize ) );
    gfx->DrawingMgr.DrawPoint( m_camPos + SVec3( 0, 0, halfSize ) );
}
