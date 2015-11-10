package ursine.editor.scene.component.inspectors.fields;

import ursine.controls.TextInput;
import ursine.editor.scene.component.ComponentDatabase;

@fieldInspector( "std::string" )
class StringFieldInspector extends FieldInspectionHandler {
    var m_string : TextInput;

    public function new(owner : ComponentInspectionHandler, instance : Dynamic, field : NativeField, type : NativeType) {
        super( owner, instance, field, type );

        m_string = new TextInput( );

        inspector.container.appendChild( m_string );
    }
}
