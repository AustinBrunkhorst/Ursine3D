package ursine.editor.scene.component.inspectors;

import ursine.controls.ComponentInspector;

import ursine.editor.scene.entity.Entity;
import ursine.editor.scene.component.ComponentDatabase;

@:keepInit
@:keepSub
class ComponentInspectionHandler {
    public var inspector : ComponentInspector;

    private var m_entity : Entity;
    private var m_component : ComponentInspection;

    public function new(entity : Entity, component : ComponentInspection) {
        m_entity = entity;
        m_component = component;

        inspector = new ComponentInspector( );

        inspector.heading = component.type;
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
