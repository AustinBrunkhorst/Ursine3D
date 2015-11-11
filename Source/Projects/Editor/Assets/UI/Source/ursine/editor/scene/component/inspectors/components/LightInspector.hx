package ursine.editor.scene.component.inspectors.components;

import ursine.editor.scene.entity.Entity;

@componentInspector( "..." )
class LightInspector extends ComponentInspectionHandler {
    public function new(entity : Entity, component : ComponentInspection) {
        super( entity, component );
    }
}
