package ursine.editor.windows;

class SceneView extends NativeCanvasWindowHandler {
    public function new() {
        super( 'SceneView' );

        window.heading = "Scene";

        onViewportInvalidated( );
    }
}
