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
    public var fieldChangeEvents : EventManager;
    public var inspector : ComponentInspector;

    private var m_entity : Entity;
    private var m_component : ComponentInspection;
    private var m_componentType : ComponentType;

    private var m_fieldHandlers : Map<String, FieldInspectionHandler>;

    public function new(entity : Entity, component : ComponentInspection) {
        m_entity = entity;
        m_component = component;
        m_componentType = Editor.instance.componentDatabase.getComponentType( m_component.type );
        m_fieldHandlers = new Map<String, FieldInspectionHandler>( );

        fieldChangeEvents = new EventManager( );
        inspector = new ComponentInspector( );

        inspector.heading = component.type;
    }

    public function updateField(name : String, value : Dynamic) {
        Reflect.setField( m_component.value, name, value );

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

    public function addButton(button : ComponentEditorButton) {
        var element = new Button( );

        // kinda hacky, but it works
        element.classList.add( 'x-component-inspector' );
        element.text = button.text;

        element.addEventListener( 'click', function() {
            m_entity.componentButtonInvoke( m_component.type, button.name );
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
        m_entity.componentFieldUpdate( m_component.type, field.name, value );
    }

    public function remove() {
        for (handler in m_fieldHandlers)
            handler.remove( );

        if (inspector.parentNode != null)
            inspector.parentNode.removeChild( inspector );
    }
}
