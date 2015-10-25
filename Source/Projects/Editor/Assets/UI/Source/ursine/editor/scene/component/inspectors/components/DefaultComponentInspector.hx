package ursine.editor.scene.component.inspectors.components;

import ursine.editor.scene.component.ComponentDatabase;
import ursine.editor.scene.entity.Entity;

class DefaultComponentInspector extends ComponentInspectionHandler {
    public function new(entity : Entity, component : ComponentInspection) {
        super( entity, component );

        var database = Editor.instance.componentDatabase;

        var componentType = database.getComponentType( component.type );

        var fields = Reflect.fields( componentType.fields );

        for (name in fields) {
            var field : NativeField = Reflect.field( componentType.fields, name );

            var instance = Reflect.field( component.value, field.name );

            var type = database.getNativeType( field.type );

            addField(
                database.createFieldInspector( this, instance, field, type )
            );
        }
    }
}
