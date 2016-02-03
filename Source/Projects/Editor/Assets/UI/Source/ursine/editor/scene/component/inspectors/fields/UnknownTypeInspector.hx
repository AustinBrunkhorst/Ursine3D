package ursine.editor.scene.component.inspectors.fields;

import ursine.editor.scene.component.ComponentDatabase;

@fieldInspector( "UNKNOWN" )
class UnknownTypeInspector extends FieldInspectionHandler {
    public function new(owner : ComponentInspectionHandler, instance : Dynamic, field : NativeField, type : NativeType) {
        super( owner, instance, field, type );

        var message = js.Browser.document.createDivElement( );

        message.classList.add( 'invalid-type' );
        message.innerHTML = 'Unknown field type. Is this type exposed in reflection?';

        inspector.container.appendChild( message );
    }
}
