package ursine.editor.windows;

import js.html.HtmlElement;
import js.html.LIElement;
import js.html.DOMElement;
import js.html.UListElement;
import ursine.native.Extern;
import ursine.editor.scene.entity.Entity;

class SceneOutline extends WindowHandler {
    private var m_entityList : UListElement;
    private var m_entityItems : Map<UInt, HtmlElement>;

    private var m_selectedItem : DOMElement = null;

    public function new() {
        super( );

        window.heading = "Outline";

        m_entityList = cast js.Browser.document.createElement( 'ul' );
        m_entityItems = new Map<UInt, HtmlElement>( );
        m_entityList.classList.add( 'entity-list' );

        window.container.appendChild( m_entityList );

        initScene( );

        Editor.instance.broadcastManager.getChannel( 'EntityManager' )
            .on( 'EntityAdded', onEntityAdded )
            .on( 'EntityRemoved', onEntityRemoved )
            .on( 'EntityNameChanged', onEntityNameChanged );

        window.addEventListener( 'keydown', onWindowKeyDown );
    }

    private function initScene() {
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

        var item = createEntityItem( entity );

        m_entityList.appendChild( item );
    }

    private function onEntityRemoved(e) {
        var item = m_entityItems[ e.uniqueID ];

        if (item == m_selectedItem)
            selectEntity( null );

        m_entityList.removeChild( item );
    }

    private function onEntityNameChanged(e) {
        var item = m_entityItems[ e.uniqueID ];

        item.innerText = e.name;
    }

    private function createEntityItem(entity : Entity) : LIElement {
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
            selectEntity( item );
        } );

        m_entityItems[ entity.uniqueID ] = cast item;

        return cast item;
    }

    private function selectEntity(item) {
        if (m_selectedItem != null)
            m_selectedItem.classList.remove( 'selected' );

        m_selectedItem = item;

        if (item == null) {
            EntityInspector.instance.inspect( null );
        } else {
            item.classList.add( 'selected' );

            EntityInspector.instance.inspect( untyped item.entity );

            untyped item.entity.select( );
        }
    }

    private function deleteSelectedEntities() {
        if (m_selectedItem == null)
            return;

        untyped m_selectedItem.entity.remove( );
    }
}
