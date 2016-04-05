package ursine.editor.scene.entity;

import ursine.native.Extern;

import ursine.editor.scene.component.ComponentInspection;

import ursine.api.events.EventManager;
import ursine.api.events.IEventContainer;

@:keepSub
class Entity implements IEventContainer {
    public var events : EventManager;

    public var uniqueID : UInt;

    private var m_handler : EntityHandler;

    public static function create() : Entity {
        return new Entity( Extern.CreateEntity( ) );
    }

    public static function createCopyName(name : String) {
        var copyMatch = ~/\(([0-9]+)\)$/i;

        // has (x)
        if (copyMatch.match( name )) {
            var index = Std.parseInt( copyMatch.matched( 1 ) );

            name = '${copyMatch.matchedLeft( )} (${index + 1})';
        // doesn't have anything, base case
        } else {
            name += ' (1)';
        }

        return name;
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

    public inline function isHierarchyChangeEnabled() : Bool {
        return m_handler.isHierarchyChangeEnabled( );
    }

    public inline function isVisibleInEditor() : Bool {
        return m_handler.isVisibleInEditor( );
    }

    public inline function setVisibleInEditor(visible : Bool) : Void {
        m_handler.setVisibleInEditor( visible );

        events.trigger( EntityEvent.EntityEditorVisibilityChanged, {
            entity: this,
            visibile: visible
        } );
    }

    public inline function enableSerialization(enable : Bool) : Void {
        return m_handler.enableSerialization( enable );
    }

    public inline function remove() : Void {
        m_handler.remove( );
    }

    public inline function getName() : String {
        return m_handler.getName( );
    }

    public inline function setName(name : String) : Void {
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

    public inline function inspect() : Array<ComponentInspection> {
        return m_handler.inspect( );
    }

    public inline function inspectComponent(component : String) : ComponentInspection {
        return m_handler.inspectComponent( component );
    }

    public inline function hasComponent(name : String) : Bool {
        return m_handler.hasComponent( name );
    }

    public inline function addComponent(name : String) : Void {
        m_handler.addComponent( name );
    }

    public inline function removeComponent(name : String) : Void {
        m_handler.removeComponent( name );
    }

    public inline function componentSet(componentName : String, value : Dynamic) : Void {
        m_handler.componentSet( componentName, value );
    }

    public inline function componentFieldUpdate(componentName : String, fieldName : String, value : Dynamic) : Void {
        m_handler.componentFieldUpdate( componentName, fieldName, value );
    }

    public inline function componentFieldArrayUpdate(componentName : String, fieldName : String, index : UInt, value : Dynamic) : Void {
        m_handler.componentFieldArrayUpdate( componentName, fieldName, index, value );
    }

    public inline function componentFieldArrayInsert(componentName : String, fieldName : String, index : UInt, value : Dynamic) : Void {
        m_handler.componentFieldArrayInsert( componentName, fieldName, index, value );
    }

    public inline function componentFieldArrayPush(componentName : String, fieldName : String, value : Dynamic) : Void {
        m_handler.componentFieldArrayInsert(
            componentName,
            fieldName,
            m_handler.componentFieldArrayGetLength( componentName, fieldName ),
            value
        );
    }

    public inline function componentFieldArrayRemove(componentName : String, fieldName : String, index : UInt) : Void {
        m_handler.componentFieldArrayRemove( componentName, fieldName, index );
    }

    public inline function componentFieldArraySwap(componentName : String, fieldName : String, index1 : UInt, index2 : UInt) : Void {
        m_handler.componentFieldArraySwap( componentName, fieldName, index1, index2 );
    }

    public inline function componentButtonInvoke(componentName : String, buttonName : String) : Void {
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

    public inline function setParent(parent : Entity) : Bool {
        return m_handler.setParent( parent == null ? null : parent.uniqueID );
    }

    public inline function getSiblingIndex() : UInt {
        return m_handler.getSiblingIndex( );
    }

    public inline function setSiblingIndex(index : UInt) : Void {
        m_handler.setSiblingIndex( index );
    }

    public inline function saveAsArchetype() : Void {
        m_handler.saveAsArchetype( );
    }

    public inline function clone() : Entity {
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
