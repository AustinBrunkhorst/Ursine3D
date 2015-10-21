package ursine.editor.scene.component.inspectors.fields;

import ursine.controls.NumberInput;
import ursine.editor.scene.component.ComponentDatabase;

@fieldInspector( "int", "float", "double" )
class NumberFieldInspector extends FieldInspectionHandler {
    public function new(owner : ComponentInspectionHandler, instance : Dynamic, field : NativeField, type : NativeType) {
        super( owner, instance, field, type );

        var number = new NumberInput( );

        number.value = m_instance;

        number.addEventListener( 'input', function() {
            // floating point
            if (m_type.name == "float" || m_type.name == "double")
                m_instance = number.valueAsNumber;
            // integral
            else
                m_instance = Std.int( number.valueAsNumber );

            m_owner.notifyChanged( m_field, m_instance );
        } );

        inspector.container.appendChild( number );
    }
}
