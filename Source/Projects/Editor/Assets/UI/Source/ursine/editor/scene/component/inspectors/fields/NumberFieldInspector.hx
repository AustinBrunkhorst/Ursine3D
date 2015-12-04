package ursine.editor.scene.component.inspectors.fields;

import ursine.controls.NumberInput;
import ursine.editor.scene.component.ComponentDatabase;

@fieldInspector( "int", "float", "double" )
class NumberFieldInspector extends FieldInspectionHandler {
    private var m_number : NumberInput;

    public function new(owner : ComponentInspectionHandler, instance : Dynamic, field : NativeField, type : NativeType) {
        super( owner, instance, field, type );

        m_number = new NumberInput( );

        m_number.addEventListener( 'change', function() {
            var value : Dynamic = m_number.valueAsNumber;

            if (Math.isNaN( value ))
                value = 0;

            // integral (not floating point)
            if (!(m_type.name == "float" || m_type.name == "double"))
                value = Std.int( value );

            m_owner.notifyChanged( m_field, value );
        } );

        // select all text on focus
        m_number.addEventListener( 'focus', function(e) {
            m_number.select( );

            e.preventDefault( );
        } );

        // initially set the value
        updateValue( instance );

        inspector.container.appendChild( m_number );
    }

    public override function updateValue(value : Dynamic) {
        var number : Dynamic;

        if (m_type.name == "float" || m_type.name == "double")
            number = untyped Math.toMaxPrecision( value, 5 );
        else
            number = Std.int( value );

        m_number.value = number;
    }
}
