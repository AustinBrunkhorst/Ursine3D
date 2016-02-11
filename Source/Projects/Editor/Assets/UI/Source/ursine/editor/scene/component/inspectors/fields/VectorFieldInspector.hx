package ursine.editor.scene.component.inspectors.fields;

import ursine.controls.NumberInput;
import ursine.editor.scene.component.ComponentDatabase;

@fieldInspector(
    "ursine::Vec2",
    "ursine::Vec3",
    "ursine::SVec3",
    "ursine::Vec4",
    "ursine::SVec4",
    "ursine::SQuat"
)
class VectorFieldInspector extends FieldInspectionHandler {
    private var m_fieldsContainer : js.html.DivElement;
    private var m_fields : Map<String, NumberInput>;

    public function new(owner : IFieldInspectionOwner, instance : Dynamic, field : NativeField, type : NativeType) {
        super( owner, instance, field, type );

        m_fieldsContainer = js.Browser.document.createDivElement( );
        {
            m_fieldsContainer.classList.add( 'vector-fields' );

            inspector.container.appendChild( m_fieldsContainer );
        }

        m_fields = new Map<String, NumberInput>( );

        for (field in type.fields) {
            createVectorField( field );
        }

        updateValue( instance );
    }

    public override function updateValue(value : Dynamic) {
        for (field in m_fields.keys( )) {
            updateVectorField( field, Reflect.field( value, field ) );
        }
    }

    private function createVectorField(field : NativeField) {
        var number = new NumberInput( );

        number.step = '0.1';

        number.addEventListener( 'change', function() {
            var value : Float = number.valueAsNumber;

            if (Math.isNaN( value ))
                value = 0;

            Reflect.setField( m_instance, field.name, value );

            notifyChanged( m_field, m_instance );
        } );

        // select all text on focus
        number.addEventListener( 'focus', function(e) {
            haxe.Timer.delay( function() {
                number.select( );
            }, 50 );

            e.preventDefault( );
        } );

        m_fields[ field.name ] = number;

        m_fieldsContainer.appendChild( number );
    }

    private function updateVectorField(name : String, value : Dynamic) {
        var field = m_fields[ name ];

        field.value = untyped Math.toMaxPrecision( value, 5 );
    }
}
