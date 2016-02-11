package ursine.editor.scene.component.inspectors.fields;

import ursine.native.Property;
import ursine.controls.NumberInput;
import ursine.editor.scene.component.ComponentDatabase;

@fieldInspector(
    "int",
    "unsigned int",
    "float",
    "double"
)
class NumberFieldInspector extends FieldInspectionHandler {
    private var m_number : NumberInput;

    public function new(owner : IFieldInspectionOwner, instance : Dynamic, field : NativeField, type : NativeType) {
        super( owner, instance, field, type );

        m_number = new NumberInput( );

        if (Reflect.hasField( field.meta, Property.InputRange ))
        {
            m_number.slider = true;

            var range = Reflect.field( field.meta, Property.InputRange );

            m_number.format = range.format;
            m_number.min = range.min;
            m_number.max = range.max;

            if (range.step > 0.0)
                m_number.step = Std.string( range.step );
        }

        // make sure we don't cause any overflow issues
        if (type.name.indexOf( "unsigned" ) != -1)
            m_number.min = "0";

        var changeHandler = function() {
            var value : Dynamic = m_number.valueAsNumber;

            if (Math.isNaN( value ))
                value = 0;

            // integral (not floating point)
            if (!(m_type.name == "float" || m_type.name == "double"))
                value = Std.int( value );

            notifyChanged( m_field, value );
        };

        m_number.addEventListener( 'change', changeHandler );

        // add events on immediate input for sliders
        if (m_number.type == 'range')
            m_number.addEventListener( 'input', changeHandler );

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
