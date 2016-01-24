package ursine.editor.scene.component.inspectors;

import ursine.utils.EventManager;
import ursine.controls.Button;
import ursine.controls.inspection.ComponentInspector;
import ursine.editor.scene.entity.Entity;
import ursine.editor.scene.component.ComponentDatabase;
import ursine.editor.scene.component.ComponentInspection;

@:keepInit
@:keepSub
class ComponentInspectionHandler {
    public var entity(default, null) : Entity;
    public var component(default, null) : ComponentInspection;

    public var fieldChangeEvents : EventManager;
    public var fieldArrayInsertEvents : EventManager;
    public var fieldArrayItemSetEvents : EventManager;
    public var fieldArrayItemRemoveEvents : EventManager;

    public var inspector : ComponentInspector;

    private var componentType : ComponentType;

    private var m_fieldHandlers : Map<String, FieldInspectionHandler>;

    public function new(entity : Entity, component : ComponentInspection) {
        this.entity = entity;
        this.component = component;

        componentType = Editor.instance.componentDatabase.getComponentType( component.type );
        m_fieldHandlers = new Map<String, FieldInspectionHandler>( );

        fieldChangeEvents = new EventManager( );
        fieldArrayInsertEvents = new EventManager( );
        fieldArrayItemSetEvents = new EventManager( );
        fieldArrayItemRemoveEvents = new EventManager( );

        inspector = new ComponentInspector( );

        inspector.heading = component.type;
    }

    public function updateField(name : String, value : Dynamic) {
        Reflect.setField( component.value, name, value );

        var result = fieldChangeEvents.trigger( name, {
            name: name,
            value: value
        } );

        if (result != false) {
            var handler : FieldInspectionHandler = m_fieldHandlers[ name ];

            if (handler != null)
                handler.updateValue( value );
        }
    }

    public function arrayInsert(name : String, index : UInt, value : Dynamic) {
        var result = fieldArrayInsertEvents.trigger( name, {
            name: name,
            index: index,
            value: value
        } );

        if (result != false) {
            var handler : FieldInspectionHandler = m_fieldHandlers[ name ];

            if (handler != null)
                handler.arrayInsert( index, value );
        }
    }

    public function arraySet(name : String, index : UInt, value : Dynamic) {
        var result = fieldArrayInsertEvents.trigger( name, {
            name: name,
            index: index,
            value: value
        } );

        if (result != false) {
            var handler : FieldInspectionHandler = m_fieldHandlers[ name ];

            if (handler != null)
                handler.arraySet( index, value );
        }
    }

    public function arrayRemove(name : String, index : UInt) {
        var result = fieldArrayInsertEvents.trigger( name, {
            name: name,
            index: index
        } );

        if (result != false) {
            var handler : FieldInspectionHandler = m_fieldHandlers[ name ];

            if (handler != null)
                handler.arrayRemove( index );
        }
    }

    public function addButton(button : ComponentEditorButton) {
        var element = new Button( );

        // kinda hacky, but it works
        element.classList.add( 'x-component-inspector' );
        element.text = button.text;

        element.addEventListener( 'click', function() {
            entity.componentButtonInvoke( component.type, button.name );
        } );

        inspector.buttons.appendChild( element );
    }

    public function addField(field : FieldInspectionHandler) {
        m_fieldHandlers[ field.name ] = field;

        inspector.fieldInspectors.appendChild( field.inspector );
    }

    public function removeField(field : FieldInspectionHandler) {
        m_fieldHandlers.remove( field.name );

        inspector.fieldInspectors.removeChild( field.inspector );
    }

    public function notifyChanged(field : NativeField, value : Dynamic) {
        entity.componentFieldUpdate( component.type, field.name, value );
    }

    public function remove() {
        for (handler in m_fieldHandlers)
            handler.remove( );

        if (inspector.parentNode != null)
            inspector.parentNode.removeChild( inspector );
    }
}
