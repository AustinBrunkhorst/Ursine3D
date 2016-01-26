package ursine.editor.scene.component.inspectors;

import ursine.controls.inspection.FieldInspector;
import ursine.editor.scene.component.ComponentDatabase;

@:keepInit
@:keepSub
class FieldInspectionHandler {
    public var name(get, null) : String;
    public var inspector : FieldInspector;

    public var arrayIndex : UInt;

    private var m_owner : ComponentInspectionHandler;
    private var m_instance : Dynamic;
    private var m_field : NativeField;
    private var m_type : NativeType;

    public function new(owner : ComponentInspectionHandler, instance : Dynamic, field : NativeField, type : NativeType) {
        m_owner = owner;
        m_instance = instance;
        m_field = field;
        m_type = type;

        inspector = new FieldInspector( );

        inspector.heading = field.name;

        arrayIndex = 0;
    }

    public function updateValue(value : Dynamic) {
        m_instance = value;
    }

    public function arrayInsert(index : UInt, value : Dynamic) { }
    public function arraySet(index : UInt, value : Dynamic) { }
    public function arrayRemove(index : UInt) { }

    public function remove() {
        if (inspector.parentNode != null)
            inspector.parentNode.removeChild( inspector );
    }

    private function notifyChanged(field : NativeField, value : Dynamic) {
        if (m_type.isArray)
            m_owner.notifyArrayChanged( m_field, arrayIndex, value );
        else
            m_owner.notifyChanged( m_field, value );
    }

    private function get_name() : String {
        return m_field.name;
    }
}
