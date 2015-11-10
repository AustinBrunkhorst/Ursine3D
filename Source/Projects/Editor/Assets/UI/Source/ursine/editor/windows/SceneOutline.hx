package ursine.editor.windows;

import js.html.HtmlElement;
import js.html.Element;
import js.html.DOMElement;
import js.html.UListElement;
import ursine.native.Extern;
import ursine.editor.scene.entity.EntityEvent;
import ursine.editor.scene.entity.Entity;

class SceneOutline extends WindowHandler {
    public static var instance : SceneOutline;

    private var m_entityList : UListElement;
    private var m_entityItems : Map<UInt, Element>;

    private var m_selectedEntities : Array<UInt> = null;

    public function new() {
        instance = this;

        super( );

        window.heading = "Outline";

        m_entityList = cast js.Browser.document.createElement( 'ul' );

        m_entityList.classList.add( 'entity-list' );

        m_entityItems = new Map<UInt, Element>( );

        m_selectedEntities = new Array<UInt>( );

        window.container.appendChild( m_entityList );

        resetScene( );

        Editor.instance.broadcastManager.getChannel( 'SceneManager' )
            .on( 'Reset', resetScene );

        Editor.instance.broadcastManager.getChannel( 'EntityManager' )
            .on( EntityEvent.EntityAdded, onEntityAdded )
            .on( EntityEvent.EntityRemoved, onEntityRemoved )
            .on( EntityEvent.EntityNameChanged, onEntityNameChanged )
            .on( EntityEvent.ComponentAdded, onComponentAdded )
            .on( EntityEvent.ComponentRemoved, onComponentRemoved );

        window.addEventListener( 'keydown', onWindowKeyDown );
    }

    public function clearSelectedEntities() {
        for (uid in m_selectedEntities) {
            var item = m_entityItems[ uid ];

            if (item == null)
                continue;

            untyped item.entity.deselect( );
        }
    }

    private function resetScene() {
        m_selectedEntities = new Array<UInt>( );
        m_entityList.innerHTML = '';
        m_entityItems = new Map<UInt, Element>( );

        EntityInspector.instance.inspect( null );

        var entities : Array<UInt> = Extern.SceneGetActiveEntities( );

        var event = { uniqueID: 0 };

        for (uniqueID in entities) {
            event.uniqueID = uniqueID;

            onEntityAdded( event );
        }
    }

    private function onWindowKeyDown(e) {
        switch (e.keyCode) {
            // delete
            case 46: {
                deleteSelectedEntities( );
            }
        }
    }

    private function onEntityAdded(e) {
        var entity = new Entity( e.uniqueID );

        if (!entity.isVisibleInEditor( )) {
            m_entityItems[ entity.uniqueID ] = null;
        } else {
            var item = createEntityItem( entity );

            m_entityList.appendChild( item );

            if (entity.hasComponent( 'Selected' ))
                selectEntity( item );
        }
    }

    private function onEntityRemoved(e) {
        var item = m_entityItems[ e.uniqueID ];

        if (item == null)
            return;

        // TODO: handle multi selection
        if (m_selectedEntities.indexOf( e.uniqueID ) != -1)
            selectEntity( null );

        m_entityList.removeChild( item );
    }

    private function onEntityNameChanged(e) {
        var item = m_entityItems[ e.uniqueID ];

        if (item == null)
            return;

        item.innerText = e.name;
    }

    private function onComponentAdded(e) {
        if (e.component == 'Selected') {
            var item = m_entityItems[ e.uniqueID ];

            if (item != null)
                selectEntity( item );
        }
    }

    private function onComponentRemoved(e) {
        if (e.component == 'Selected') {
            var item = m_entityItems[ e.uniqueID ];

            if (item != null)
                deselectEntity( item );
        }
    }

    private function createEntityItem(entity : Entity) : Element {
        var item = js.Browser.document.createElement( 'li' );

        item.addEventListener( 'dblclick', function() {
            item.contentEditable = 'true';

            var range = js.Browser.document.createRange( );

            range.selectNodeContents( item );

            var selection = js.Browser.window.getSelection( );

            selection.removeAllRanges( );
            selection.addRange( range );
        } );

        item.addEventListener( 'keydown', function(e) {
            // return key
            if (e.keyCode == 13) {
                item.blur( );

                e.preventDefault( );

                return false;
            }

            return true;
        } );

        item.addEventListener( 'blur', function() {
            item.contentEditable = 'false';

            entity.setName( item.innerText );
        } );

        item.innerText = entity.getName( );

        untyped item.entity = entity;

        item.addEventListener( 'click', function(e) {
            clearSelectedEntities( );

            untyped item.entity.select( );
        } );

        m_entityItems[ entity.uniqueID ] = cast item;

        return item;
    }

    private function selectEntity(item : Element) {
        if (item == null) {
            EntityInspector.instance.inspect( null );
        } else {
            item.classList.add( 'selected' );

            untyped item.scrollIntoViewIfNeeded( );

            // TODO: handle multi selection
            EntityInspector.instance.inspect( untyped item.entity );

            m_selectedEntities.push( untyped item.entity.uniqueID );
        }
    }

    private function deselectEntity(item : Element) {
        if (item != null) {
            item.classList.remove( 'selected' );

            // TODO: handle multi selection
            EntityInspector.instance.inspect( null );

            m_selectedEntities.filter( function(x) {
                return x == untyped item.entity.uniqueID;
            } );
        }
    }

    private function deleteSelectedEntities() {
        for (uid in m_selectedEntities) {
            var item = m_entityItems[ uid ];

            if (item == null)
                continue;

            var entity : Entity = untyped item.entity;

            if (entity.isRemovalEnabled( )) {
                entity.remove( );
            } else {
                // TODO: add removal warning
            }
        }
    }
}
