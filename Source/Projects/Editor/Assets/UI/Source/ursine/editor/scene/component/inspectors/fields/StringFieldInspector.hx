package ursine.editor.scene.component.inspectors.fields;

import ursine.editor.scene.component.ComponentDatabase;

@fieldInspector( "std::string" )
class StringFieldInspector extends FieldInspectionHandler {
    public function new(owner : ComponentInspectionHandler, instance : Dynamic, field : NativeField, type : NativeType) {
        super( owner, instance, field, type );
    }
}
