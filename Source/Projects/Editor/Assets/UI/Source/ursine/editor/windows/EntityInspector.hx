package ursine.editor.windows;

class EntityInspector extends WindowHandler {
    public function new() {
        super( );

        window.heading = "Inspector";

        window.style.top = "0";
        window.style.bottom = "50%";
        window.style.left = "0";
        window.style.width = "15%";
    }
}
