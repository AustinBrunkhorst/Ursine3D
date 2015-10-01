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
        .On( MM_SCROLL, &SceneView::onMouseScroll )
        .On( MM_MOVE, &SceneView::onMouseMove );

    GetCoreSystem( KeyboardManager )->Listener( this )
        .On( KM_KEY_DOWN, &SceneView::onKeyDown );
}

SceneView::~SceneView(void)
{
    GetCoreSystem( MouseManager )->Listener( this )
        .Off( MM_SCROLL, &SceneView::onMouseScroll )
        .Off( MM_MOVE, &SceneView::onMouseMove );

    GetCoreSystem( KeyboardManager )->Listener( this )
        .Off( KM_KEY_DOWN, &SceneView::onKeyDown );
}

GFXCamera &SceneView::getEditorCamera(void)
{
    return GetCoreSystem( GfxAPI )->CameraMgr.GetCamera( 
        m_editor->GetProject( )->GetScene( ).GetEditorCamera( )
    );
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

    if (!m_isFocused)
        return;

    m_camZoom -= args->delta.Y( );

    if (m_camZoom < 1.0f) 
        m_camZoom = 1.0f;

    auto &camera = getEditorCamera( );

    auto look = camera.GetLook( );

    // normalize look and scale by zoom
    look.Normalize( );
    look = look * m_camZoom;

    // negate the vector, opposite of look is going away from center
    camera.SetPosition( camera.GetPosition( ) - look );
}

void SceneView::onMouseMove(EVENT_HANDLER(MouseManager))
{
    EVENT_ATTRS(MouseManager, MouseMoveArgs);

    // ignore is not focused for pressing alt is down
    if (!m_isFocused)
        return;

    // must have ctrl key down
    if (!(GetCoreSystem( KeyboardManager )->GetModifiers( ) & KMD_CTRL ))
        return;

    auto &camera = getEditorCamera( );

    auto look = camera.GetLook( );

    auto positionDelta = args->positionDelta;

    auto camTransform = camera.GetViewMatrix( );

    // we need to limit the up delta so that we can't wrap if we are at the very top/bottom
    positionDelta /= 2.f;

    // must have left button down
    if (sender->IsButtonDown( MBTN_LEFT ))
    {
        if (positionDelta.Length( ) > 0)
        {
            // generate the up rotation
            auto upRotation = SQuat( 
                positionDelta.Y( ), 
                camTransform.TransformVector( SVec3( 1.0f, 0.0f, 0.0f ) ) 
            );

            // generate side rotation
            auto sideRotation = SQuat( 
                positionDelta.X( ), 
                SVec3( 0.0f, 1.0f, 0.0f ) 
            );

            // transform w/ rotations
            look = sideRotation.Rotate( look );
            look = upRotation.Rotate( look );

            camera.SetLook( look );
        }
    }
}

void SceneView::onKeyDown(EVENT_HANDLER(KeyboardManager))
{
    EVENT_ATTRS(KeyboardManager, KeyboardKeyArgs);

    // the scene view doesn't have focus
    if (!m_isFocused)
        return;

    float speed = 3;

    auto &camera = getEditorCamera( );

    auto look = camera.GetLook( );
    auto right = camera.GetRight( );

    auto position = camera.GetPosition( );

    auto up = camera.GetUp( );

    auto direction = SVec3( 0.0f, 0.0f, 0.0f );

    switch (args->key)
    {
    case KEY_W:
        direction += look;
        break;
    case KEY_S:
        direction -= look;
        break;
    case KEY_A:
        direction += right;
        break;
    case KEY_D:
        direction -= right;
        break;
    case KEY_E:
        direction += up;
        break;
    case KEY_Q:
        direction -= up;
        break;
    default:
        break;
    }

    // make sure something happened
    if (direction.Length( ) > 0.0f)
    {
        // normalize vector, scale by speed
        direction.Normalize( );
        direction *= speed;

        // apply to position
        position += direction;

        camera.SetPosition( position - look );
    }
}