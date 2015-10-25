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
            var number : Float = number.valueAsNumber;

            if (Math.isNaN( number ))
                number = 0;

            // floating point
            if (m_type.name == "float" || m_type.name == "double")
                m_instance = number;
            // integral
            else
                m_instance = Std.int( number );

            m_owner.notifyChanged( m_field, m_instance );
        } );

        // select all text on focus
        number.addEventListener( 'focus', function(e) {
            number.select( );

            e.preventDefault( );
        } );

        inspector.container.appendChild( number );
    }
}
