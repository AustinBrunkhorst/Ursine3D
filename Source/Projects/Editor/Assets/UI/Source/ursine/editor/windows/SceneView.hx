package ursine.editor.windows;

class SceneView extends WindowHandler {
    public function new() {
        super( );

        window.heading = "Scene";

        window.style.top = "0";
        window.style.bottom = "0";
        window.style.left = "15%";
        window.style.right = "0";

        trace( "testing" );
    }
}
