#include "Precompiled.h"

#include "SceneView.h"

#include "Project.h"

#include "EditorCameraSystem.h"

#include <SystemManager.h>

using namespace ursine;
using namespace native_tool;

SceneView::SceneView(JSHandlerArgs)
    : m_editor( GetCoreSystem( Editor ) )
{
    Listener( this )
        .On( event::Focus, &SceneView::onFocusChanged )
        .On( event::Blur, &SceneView::onFocusChanged )
        .On( event::MouseOver, &SceneView::onMouseFocusChanged )
        .On( event::MouseOut, &SceneView::onMouseFocusChanged )
        .On( event::ViewportInvalidated, &SceneView::onViewportInvalidated );
}

SceneView::~SceneView(void)
{
    Listener( this )
        .Off( event::Focus, &SceneView::onFocusChanged )
        .Off( event::Blur, &SceneView::onFocusChanged )
        .Off( event::MouseOver, &SceneView::onMouseFocusChanged )
        .Off( event::MouseOut, &SceneView::onMouseFocusChanged )
        .Off( event::ViewportInvalidated, &SceneView::onViewportInvalidated );
}

void SceneView::onFocusChanged(EVENT_HANDLER(NativeEditorTool))
{
    EVENT_ATTRS(NativeEditorTool, NativeToolEvent);

    auto world = m_editor->GetProject( )->GetScene( )->GetWorld( );

    auto focused = (args->name == event::Focus);

    if (world->HasEntitySystem( EditorCameraSystem ))
    {
        world->GetEntitySystem( EditorCameraSystem )->SetFocus( focused );
    }
}

void SceneView::onMouseFocusChanged(EVENT_HANDLER(NativeEditorTool))
{
    EVENT_ATTRS(NativeEditorTool, NativeToolEvent);

    auto world = m_editor->GetProject( )->GetScene( )->GetWorld( );

    auto focused = (args->name == event::MouseOver);

    if (world->HasEntitySystem( EditorCameraSystem ))
    {
        world->GetEntitySystem( EditorCameraSystem )->SetMouseFocus( focused );
    }
}

void SceneView::onViewportInvalidated(EVENT_HANDLER(NativeEditorTool))
{
    EVENT_ATTRS(NativeEditorTool, NativeToolEvent);

    auto scene = m_editor->GetProject( )->GetScene( );

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

    auto handle = scene->GetViewport( );

    auto &viewport =
        GetCoreSystem( graphics::GfxAPI )->ViewportMgr.GetViewport( handle );

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