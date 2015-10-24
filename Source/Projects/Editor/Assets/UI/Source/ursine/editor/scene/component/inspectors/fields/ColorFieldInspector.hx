package ursine.editor.scene.component.inspectors.fields;

import ursine.controls.NumberInput;
import ursine.editor.scene.component.ComponentDatabase;

@fieldInspector( "ursine::Color" )
class ColorFieldInspector extends FieldInspectionHandler {
    public function new(owner : ComponentInspectionHandler, instance : Dynamic, field : NativeField, type : NativeType) {
        super( owner, instance, field, type );

        var fields = Reflect.fields( type.fields );

        for (name in fields) {
            var colorField = Reflect.field( type.fields, name );

            createColorField( colorField );
        }
    }

    private function createColorField(field : NativeField) {
        var number = new NumberInput( );

        number.step = '1';
        number.min = '0';
        number.max = '255';

        number.value = Std.string( Reflect.field( m_instance, field.name ) * 255 );

        number.addEventListener( 'input', function() {
            Reflect.setField( m_instance, field.name, number.valueAsNumber / 255.0 );

            m_owner.notifyChanged( m_field, m_instance );
        } );

        inspector.container.appendChild( number );
    }
}
