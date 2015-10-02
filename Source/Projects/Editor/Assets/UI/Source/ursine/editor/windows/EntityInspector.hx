package ursine.editor.windows;

import ursine.editor.scene.Entity;

class EntityInspector extends WindowHandler {
    public static var instance : EntityInspector;

    public function new() {
        instance = this;

        super( );

        window.heading = "Inspector";

        window.style.top = "0";
        window.style.bottom = "50%";
        window.style.left = "0";
        window.style.width = "15%";

        // TODO:
        window.style.borderBottom = "none";
    }

    public function inspect(entity : Entity) {
        var inspection = entity.inspect( );

        trace( inspection );
    }
}
