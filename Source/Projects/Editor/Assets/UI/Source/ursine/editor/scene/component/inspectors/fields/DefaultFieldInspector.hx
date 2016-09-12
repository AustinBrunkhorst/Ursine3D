package ursine.editor.scene.component.inspectors.fields;

import ursine.controls.ComboInput;
import ursine.editor.scene.component.ComponentDatabase;

class DefaultFieldInspector extends FieldInspectionHandler implements IFieldInspectionOwner {
    private var m_arrayInspector : ArrayTypeInspector;
    private var m_isEnum : Bool;
    private var m_isBitMaskEditor : Bool;
    private var m_comboInput : ComboInput;
    private var m_enumValueOptions : Map<String, js.html.OptionElement>;
    private var m_structFieldHandlers : Map<String, FieldInspectionHandler>;

    public function new(owner : IFieldInspectionOwner, instance : Dynamic, field : NativeField, type : NativeType) {
        super( owner, instance, field, type );

        if (type.isArray) {
            initArray( );
        } else if (type.enumValue != null) {
            initEnum( );
        } else {
            initDefaultStruct( );
        }

        updateValue( instance );
    }

    public override function updateValue(value : Dynamic) {
        m_instance = value;

        if (m_isEnum) {
            if (m_isBitMaskEditor) {
                loadEnumBitMaskValue( value );
            } else {
                m_comboInput.value = Editor.instance.componentDatabase.getEnumNumberValue(
                    m_type.enumValue,
                    value
                );
            }
        } else if (m_arrayInspector != null) {
            m_arrayInspector.updateValue( value );
        } else {
            for (field in m_type.fields) {
                var handler : FieldInspectionHandler = m_structFieldHandlers[ field.name ];

                if (handler == null)
                    throw 'Unable to find handler for field ${field.name}';

                var fieldInstance = Reflect.field( value, field.name );

                handler.updateValue( fieldInstance );
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

    public function getFieldHandlers() : Array<FieldInspectionHandler> {
        var handlers = [ ];

        for (handler in m_structFieldHandlers)
            handlers.push( handler );

        return handlers;
    }

    public function ownerNotifyChanged(handler : FieldInspectionHandler, field : NativeField, value : Dynamic) : Void {
        if (m_isEnum) {
            super.notifyChanged( field, value );
        } else {
            Reflect.setField( m_instance, field.name, value );

            m_owner.ownerNotifyChanged( this, m_field, m_instance );
        }
    }

    private function initArray() {
        m_arrayInspector = new ArrayTypeInspector( inspector, cast m_owner, m_instance, m_field, m_type );
    }

    private function initEnum() {
        m_isEnum = true;
        m_comboInput = new ComboInput( );
        m_enumValueOptions = new Map<String, js.html.OptionElement>( );

        for (entry in m_type.enumValue) {
            var option = js.Browser.document.createOptionElement( );

            option.text = entry.key;
            option.value = entry.value;

            m_comboInput.appendChild( option );

            m_enumValueOptions[ entry.key ] = option;
        }

        m_isBitMaskEditor = Reflect.field( m_field.meta, 'BitMaskEditor' ) != null;

        if (m_isBitMaskEditor) {
            m_comboInput.multiple = true;
            m_comboInput.size = Std.int( Math.min( 10, m_type.enumValue.length ) );
        }

        m_comboInput.addEventListener( 'change', function(e) {
            m_instance = getEnumBitMaskValue( );

            notifyChanged( m_field, m_instance );
        } );

        inspector.container.appendChild( m_comboInput );
    }

    private function initDefaultStruct( ) {
        inspector.classList.add( 'default-struct' );

        m_structFieldHandlers = new Map<String, FieldInspectionHandler>( );

        var componentOwner : ComponentInspectionHandler = cast m_owner;

        var database = Editor.instance.componentDatabase;

        for (field in m_type.fields) {
            var fieldInstance = Reflect.field( m_instance, field.name );
            var fieldType = database.getNativeType( field.type );

            var fieldInspector = database.createFieldInspector(
                cast this,
                fieldInstance,
                field,
                fieldType
            );

            inspector.container.appendChild( fieldInspector.inspector );

            m_structFieldHandlers[ field.name ] = fieldInspector;
        }
    }

    private function loadEnumBitMaskValue(value : Dynamic) : Void {
        for (entry in m_type.enumValue) {
            var option = m_enumValueOptions[ entry.key ];

            option.selected = (value & entry.value) == entry.value;
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
