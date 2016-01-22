package ursine.editor.windows;

class SceneView extends NativeCanvasWindowHandler {
    public function new() {
        super( 'SceneView' );

        window.heading = "Scene";

        onViewportInvalidated( );

        window.addEventListener( 'keydown', onWindowKeyDown );
    }

    private function onWindowKeyDown(e) {
        switch (e.keyCode) {
            // delete
            case 46: {
                SceneOutline.instance.deleteSelectedEntities( );
            }
        }
    }
}
