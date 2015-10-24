package ursine.editor.scene.component.inspectors;

import ursine.utils.EventManager;
import ursine.controls.ComponentInspector;
import ursine.editor.scene.entity.Entity;
import ursine.editor.scene.component.ComponentDatabase;

@:keepInit
@:keepSub
class ComponentInspectionHandler {
    public var fieldChangeEvents : EventManager;
    public var inspector : ComponentInspector;

    private var m_entity : Entity;
    private var m_component : ComponentInspection;
    private var m_componentType : ComponentType;

    public function new(entity : Entity, component : ComponentInspection) {
        m_entity = entity;
        m_component = component;
        m_componentType = Editor.instance.componentDatabase.getComponentType( m_component.type );

        fieldChangeEvents = new EventManager( );
        inspector = new ComponentInspector( );

        inspector.heading = component.type;
    }

    public function updateField(name : String, value : Dynamic) {
        Reflect.setField( m_component.value, name, value );

        fieldChangeEvents.trigger( name, {
            name: name,
            value: value
        } );
    }

    public function addField(field : FieldInspectionHandler) {
        inspector.fieldInspectors.appendChild( field.inspector );
    }

    public function notifyChanged(field : NativeField, value : Dynamic) {
        m_entity.updateComponentField( m_component.type, field.name, value );
    }

    public function remove() {
        inspector.parentNode.removeChild( inspector );
    }
}
