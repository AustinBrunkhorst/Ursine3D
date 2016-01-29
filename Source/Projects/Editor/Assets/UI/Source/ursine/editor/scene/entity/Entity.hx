package ursine.editor.scene.entity;

import ursine.controls.PolymerElement;
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

    public static function createFromArchetype() : Void {
        Extern.CreateEntityFromArchetype( );
    }

    public function new(uniqueID : UInt) {
        events = new EventManager( );

        this.uniqueID = uniqueID;

        m_handler = new EntityHandler( uniqueID );

        Editor.instance.broadcastManager.getChannel( 'EntityManager' )
            .on( EntityEvent.ComponentAdded, onComponentAdded )
            .on( EntityEvent.ComponentRemoved, onComponentRemoved )
            .on( EntityEvent.ComponentChanged, onComponentChanged )
            .on( EntityEvent.ComponentArrayInserted, onComponentArrayInserted )
            .on( EntityEvent.ComponentArraySet, onComponentArraySet )
            .on( EntityEvent.ComponentArrayRemove, onComponentArrayRemove );
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

    public function componentFieldUpdate(componentName : String, fieldName : String, value : Dynamic) : Void {
        m_handler.componentFieldUpdate( componentName, fieldName, value );
    }

    public function componentFieldArrayUpdate(componentName : String, fieldName : String, index : UInt, value : Dynamic) : Void {
        m_handler.componentFieldArrayUpdate( componentName, fieldName, index, value );
    }

    public function componentFieldArrayInsert(componentName : String, fieldName : String, index : UInt, value : Dynamic) : Void {
        m_handler.componentFieldArrayInsert( componentName, fieldName, index, value );
    }

    public function componentFieldArrayPush(componentName : String, fieldName : String, value : Dynamic) : Void {
        m_handler.componentFieldArrayInsert(
            componentName,
            fieldName,
            m_handler.componentFieldArrayGetLength( componentName, fieldName ),
            value
        );
    }

    public function componentFieldArrayRemove(componentName : String, fieldName : String, index : UInt) : Void {
        m_handler.componentFieldArrayRemove( componentName, fieldName, index );
    }

    public function componentButtonInvoke(componentName : String, buttonName : String) : Void {
        m_handler.componentButtonInvoke( componentName, buttonName );
    }

    public function getChildren() : Array<Entity> {
        var children : Dynamic = m_handler.getChildren( );

        if (children == false)
            return [ ];

        return children.map( function(uid) {
            return new Entity( uid );
        } );
    }

    public function getParent() : Entity {
        var parentUniqueID = m_handler.getParent( );

        return parentUniqueID == null ? null : new Entity( parentUniqueID );
    }

    public function setParent(parent : Entity) : Bool {
        return m_handler.setParent( parent == null ? null : parent.uniqueID );
    }

    public function getSiblingIndex() : UInt {
        return m_handler.getSiblingIndex( );
    }

    public function setSiblingIndex(index : UInt) : Void {
        m_handler.setSiblingIndex( index );
    }

    public function saveAsArchetype() : Void {
        m_handler.saveAsArchetype( );
    }

    public function clone() : Entity {
        return new Entity( m_handler.clone( ) );
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

    private function onComponentArrayInserted(e) {
        if (e.uniqueID == uniqueID)
            events.trigger( EntityEvent.ComponentArrayInserted, e );
    }

    private function onComponentArraySet(e) {
        if (e.uniqueID == uniqueID)
            events.trigger( EntityEvent.ComponentArraySet, e );
    }

    private function onComponentArrayRemove(e) {
        if (e.uniqueID == uniqueID)
            events.trigger( EntityEvent.ComponentArrayRemove, e );
    }
}
