package ursine.editor.scene.entity;

import ursine.native.Extern;

import ursine.editor.scene.component.ComponentInspection;

import ursine.utils.EventManager;
import ursine.utils.IEventContainer;

@:keepSub
class Entity implements IEventContainer {
    public var events : EventManager;

    public var uniqueID : UInt;

    private var m_handler : EntityHandler;

    public static function create() : Entity {
        return new Entity( Extern.CreateEntity( ) );
    }

    public function new(uniqueID : UInt) {
        events = new EventManager( );

        this.uniqueID = uniqueID;

        m_handler = new EntityHandler( uniqueID );

        Editor.instance.broadcastManager.getChannel( 'EntityManager' )
            .on( EntityEvent.ComponentAdded, onComponentAdded )
            .on( EntityEvent.ComponentRemoved, onComponentRemoved )
            .on( EntityEvent.ComponentChanged, onComponentChanged );
    }

    public function isValid() : Bool {
        return m_handler.isValid( );
    }

    public function isRemovalEnabled() : Bool {
        return m_handler.isRemovalEnabled( );
    }

    public function isHierarchyChangeEnabled() : Bool {
        return m_handler.isHierarchyChangeEnabled( );
    }

    public function isVisibleInEditor() : Bool {
        return m_handler.isVisibleInEditor( );
    }

    public function remove() : Void {
        m_handler.remove( );
    }

    public function getName() : String {
        return m_handler.getName( );
    }

    public function setName(name : String) : Void {
        m_handler.setName( name );
    }

    public function select() : Bool {
        if (!m_handler.hasComponent( 'Selected' )) {
            m_handler.addComponent( 'Selected' );

            return true;
        }

        return false;
    }

    public function deselect() : Void {
        if (m_handler.hasComponent( 'Selected' ))
            m_handler.removeComponent( 'Selected' );
    }

    public function inspect() : Array<ComponentInspection> {
        return m_handler.inspect( );
    }

    public function hasComponent(name : String) : Bool {
        return m_handler.hasComponent( name );
    }

    public function addComponent(name : String) : Void {
        m_handler.addComponent( name );
    }

    public function removeComponent(name : String) : Void {
        m_handler.removeComponent( name );
    }

    public function updateComponentField(componentName : String, fieldName : String, value : Dynamic) : Void {
        m_handler.updateComponentField( componentName, fieldName, value );
    }

    private function onComponentAdded(e) {
        if (e.uniqueID == uniqueID)
            events.trigger( EntityEvent.ComponentAdded, e );
    }

    private function onComponentRemoved(e) {
        if (e.uniqueID == uniqueID)
            events.trigger( EntityEvent.ComponentRemoved, e );
    }

    private function onComponentChanged(e) {
        if (e.uniqueID == uniqueID)
            events.trigger( EntityEvent.ComponentChanged, e );
    }
}
