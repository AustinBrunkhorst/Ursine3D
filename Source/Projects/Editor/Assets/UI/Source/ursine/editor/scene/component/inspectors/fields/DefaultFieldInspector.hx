package ursine.editor.scene.component.inspectors.fields;

import ursine.controls.ComboInput;
import ursine.editor.scene.component.ComponentDatabase;

class DefaultFieldInspector extends FieldInspectionHandler {
    private var m_arrayInspector : ArrayTypeInspector;
    private var m_isEnum : Bool;
    private var m_isBitMaskEditor : Bool;
    private var m_comboInput : ComboInput;
    private var m_enumValueOptions : Map<String, js.html.OptionElement>;

    public function new(owner : ComponentInspectionHandler, instance : Dynamic, field : NativeField, type : NativeType) {
        super( owner, instance, field, type );


        if (type.isArray) {
            initArray( );
        } else if (type.enumValue != null) {
            initEnum( );
        }
    }

    public override function updateValue(value : Dynamic) {
        m_instance = value;

        if (m_isEnum) {
            if (m_isBitMaskEditor) {
                loadEnumBitMaskValue( value );
            } else {
                m_comboInput.value = value;
            }
        }
    }

    public override function arrayInsert(index : UInt, value : Dynamic) {
        if (m_arrayInspector != null)
            m_arrayInspector.arrayInsert( index, value );
    }

    public override function arraySet(index : UInt, value : Dynamic) {
        if (m_arrayInspector != null)
            m_arrayInspector.arraySet( index, value );
    }

    public override function arrayRemove(index : UInt) {
        if (m_arrayInspector != null)
            m_arrayInspector.arrayRemove( index );
    }

    private function initArray() {
        m_arrayInspector = new ArrayTypeInspector( inspector, m_owner, m_instance, m_field, m_type );
    }

    private function initEnum() {
        m_isEnum = true;
        m_comboInput = new ComboInput( );
        m_enumValueOptions = new Map<String, js.html.OptionElement>( );

        var values = Reflect.fields( m_type.enumValue );

        for (key in values) {
            var option = js.Browser.document.createOptionElement( );

            option.text = key;
            option.value = Reflect.field( m_type.enumValue, key );

            m_comboInput.appendChild( option );

            m_enumValueOptions[ key ] = option;
        }

        m_isBitMaskEditor = Reflect.field( m_field.meta, 'BitMaskEditor' ) != null;

        if (m_isBitMaskEditor) {
            m_comboInput.multiple = true;
            m_comboInput.size = Std.int( Math.min( 10, values.length ) );
        }

        m_comboInput.addEventListener( 'change', function(e) {
            m_instance = getEnumBitMaskValue( );

            m_owner.notifyChanged( m_field, m_instance );
        } );

        inspector.container.appendChild( m_comboInput );

        updateValue( m_instance );
    }

    private function loadEnumBitMaskValue(value : Dynamic) : Void {
        var values = Reflect.fields( m_type.enumValue );

        for (key in values) {
            var option = m_enumValueOptions[ key ];

            var keyValue : Int = Reflect.field( m_type.enumValue, key );

            option.selected = (value & keyValue) == keyValue;
        }
    }

    private function getEnumBitMaskValue() : Int {
        var value : Int = 0;

        var values = m_comboInput.selectedOptions;

        for (option in values) {
            if (untyped option.selected) {
                value |= Std.parseInt( untyped option.value );
            }
        }

        return value;
    }
}
