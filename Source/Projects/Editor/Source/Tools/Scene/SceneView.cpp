#include "Precompiled.h"

#include "SceneView.h"

#include "Project.h"

#include <MouseManager.h>
#include <KeyboardManager.h>

using namespace ursine;

SceneView::SceneView(JSHandlerArgs)
    : m_editor( GetCoreSystem( Editor ) )
{
    Listener( this )
        .On( "viewportInvalidated", &SceneView::onViewportInvalidated );
}

SceneView::~SceneView(void)
{
    Listener( this )
        .Off( "viewportInvalidated", &SceneView::onViewportInvalidated );
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
            GetCoreSystem(graphics::GfxAPI )->ViewportMgr.GetViewport( handle );

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