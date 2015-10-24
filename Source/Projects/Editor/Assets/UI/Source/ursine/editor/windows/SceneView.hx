package ursine.editor.windows;

class SceneView extends NativeCanvasWindowHandler {
    public function new() {
        super( 'SceneView' );

        window.heading = "Scene";

        window.style.top = "0";
        window.style.bottom = "0";
        window.style.left = "15%";
        window.style.right = "0";

        // TODO:
        window.style.borderLeft = "none";

        //onViewportInvalidated( );
    }
}
