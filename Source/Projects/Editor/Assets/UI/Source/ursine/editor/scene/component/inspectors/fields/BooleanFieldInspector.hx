package ursine.editor.scene.component.inspectors.fields;

import ursine.controls.CheckBoxInput;
import ursine.editor.scene.component.ComponentDatabase;

@fieldInspector( "bool" )
class BooleanFieldInspector extends FieldInspectionHandler {
    private var m_bool : CheckBoxInput;

    public function new(owner : ComponentInspectionHandler, instance : Dynamic, field : NativeField, type : NativeType) {
        super( owner, instance, field, type );

        m_bool = new CheckBoxInput( );

        m_bool.addEventListener( 'change', function() {
            notifyChanged( m_field, m_bool.checked );
        } );

        inspector.container.appendChild( m_bool );

        updateValue( instance );
    }

    public override function updateValue(value : Dynamic) {
        m_bool.checked = value;

        m_instance = value;
    }
}
