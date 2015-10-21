package ursine.editor.scene.component.inspectors.fields;

import ursine.controls.NumberInput;
import ursine.editor.scene.component.ComponentDatabase;

@fieldInspector(
    "ursine::Vec2",
    "ursine::Vec3",
    "ursine::SVec3",
    "ursine::Vec4",
    "ursine::SVec4"
)
class VectorFieldInspector extends FieldInspectionHandler {
    public function new(owner : ComponentInspectionHandler, instance : Dynamic, field : NativeField, type : NativeType) {
        super( owner, instance, field, type );

        var fields = Reflect.fields( type.fields );

        for (name in fields) {
            var vectorField = Reflect.field( type.fields, name );

            createVectorField( vectorField );
        }
    }

    private function createVectorField(field : NativeField) {
        var number = new NumberInput( );

        number.step = '0.1';
        number.value = Reflect.field( m_instance, field.name );

        number.addEventListener( 'input', function() {
            var number : Float = number.valueAsNumber;

            if (Math.isNaN( number ))
                number = 0;

            Reflect.setField( m_instance, field.name, number );

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
