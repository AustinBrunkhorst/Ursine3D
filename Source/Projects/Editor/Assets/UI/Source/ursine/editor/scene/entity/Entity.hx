package ursine.editor.scene.entity;

import ursine.editor.scene.component.ComponentInspection;

import ursine.utils.EventManager;
import ursine.utils.IEventContainer;

class Entity implements IEventContainer {
    public var events : EventManager;

    public var uniqueID : UInt;

    private var m_handler : EntityHandler;

    public function new(uniqueID : UInt) {
        events = new EventManager( );

        this.uniqueID = uniqueID;

        m_handler = new EntityHandler( uniqueID );

        Editor.instance.broadcastManager.getChannel( 'EntityManager' )
            .on( EntityEvent.ComponentChanged, onComponentChanged );
    }

    public function isValid() : Bool {
        return m_handler.isValid( );
    }

    public function getName() : String {
        return m_handler.getName( );
    }

    public function inspect() : Array<ComponentInspection> {
        return m_handler.inspect( );
    }

    public function updateComponentField(componentName : String, fieldName : String, value : Dynamic) : Void {
        m_handler.updateComponentField( componentName, fieldName, value );
    }

    private function onComponentChanged(e) {
        if (e.uniqueID == uniqueID)
            events.trigger( EntityEvent.ComponentChanged, e );
    }
}
