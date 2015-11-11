package ursine.editor.scene.component.inspectors.fields;

import js.Browser;
import ursine.editor.scene.component.ComponentDatabase;

class DefaultFieldInspector extends FieldInspectionHandler {
    public function new(owner : ComponentInspectionHandler, instance : Dynamic, field : NativeField, type : NativeType) {
        super( owner, instance, field, type );

        if (type.enumValue != null) {
            initEnum( );
        }
    }

    private function initEnum() {
        var select = Browser.document.createSelectElement( );

        var values = Reflect.fields( m_type.enumValue );

        for (key in values) {
            var option = Browser.document.createOptionElement( );

            option.text = key;
            option.value = Reflect.field( m_type.enumValue, key );

            select.appendChild( option );
        }

        inspector.container.appendChild( select );
    }
}
