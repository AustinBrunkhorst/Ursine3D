package ursine.editor.scene.component.inspectors.fields;

import ursine.controls.TextInput;
import ursine.editor.scene.component.ComponentDatabase;

@fieldInspector( "std::string" )
class StringFieldInspector extends FieldInspectionHandler {
    var m_string : TextInput;

    public function new(owner : IFieldInspectionOwner, instance : Dynamic, field : NativeField, type : NativeType) {
        super( owner, instance, field, type );

        m_string = new TextInput( );

        m_string.addEventListener( 'change', function() {
            notifyChanged( m_field, m_string.value );
        } );

        m_string.addEventListener( 'focus', function(e) {
            haxe.Timer.delay( function() {
                m_string.select( );
            }, 50 );

            e.preventDefault( );
        } );

        inspector.container.appendChild( m_string );

        updateValue( instance );
    }

    public override function updateValue(value : Dynamic) {
        m_string.value = value;

        m_instance = value;
    }
}
