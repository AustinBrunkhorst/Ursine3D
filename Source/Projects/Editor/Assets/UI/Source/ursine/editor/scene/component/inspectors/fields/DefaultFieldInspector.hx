package ursine.editor.scene.component.inspectors.fields;

import ursine.controls.ComboInput;
import ursine.editor.scene.component.ComponentDatabase;

class DefaultFieldInspector extends FieldInspectionHandler {
    public function new(owner : ComponentInspectionHandler, instance : Dynamic, field : NativeField, type : NativeType) {
        super( owner, instance, field, type );

        // handle enum types
        if (type.enumValue != null) {
            initEnum( );
        }
    }

    private function initEnum() {
        var combo = new ComboInput( );

        var values = Reflect.fields( m_type.enumValue );

        for (key in values) {
            var option = js.Browser.document.createOptionElement( );

            option.text = key;
            option.value = Reflect.field( m_type.enumValue, key );

            combo.appendChild( option );
        }

        combo.value = m_instance;

        combo.addEventListener( 'change', function(e) {
            m_instance = Std.parseInt( combo.value );

            m_owner.notifyChanged( m_field, m_instance );
        } );

        inspector.container.appendChild( combo );
    }
}
