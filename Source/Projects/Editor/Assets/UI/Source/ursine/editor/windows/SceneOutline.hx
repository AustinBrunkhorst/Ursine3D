package ursine.editor.windows;

import js.html.LIElement;
import js.html.DOMElement;
import js.html.UListElement;
import ursine.native.Extern;
import ursine.editor.scene.entity.Entity;

class SceneOutline extends WindowHandler {
    private var m_entityList : UListElement;
    private var m_selectedItem : DOMElement = null;

    public function new() {
        super( );

        window.heading = "Outline";

        m_entityList = cast js.Browser.document.createElement( 'ul' );
        m_entityList.classList.add( 'entity-list' );

        window.container.appendChild( m_entityList );

        // TODO:
        /*initScene( );

        Editor.instance.broadcastManager.getChannel( 'EntityManager' )
            .on( 'EntityAdded', onEntityAdded );*/
    }

    private function initScene() {
        var entities : Array<UInt> = Extern.SceneGetActiveEntities( );

        var event = { uniqueID: 0 };

        for (uniqueID in entities) {
            event.uniqueID = uniqueID;

            onEntityAdded( event );
        }
    }

    private function onEntityAdded(e) {
        var entity = new Entity( e.uniqueID );

        var item = createEntityItem( entity );

        m_entityList.appendChild(item );
    }

    private function createEntityItem(entity : Entity) : LIElement {
        var item = js.Browser.document.createElement( 'li' );

        item.innerText = entity.getName( );

        untyped item.entity = entity;

        item.addEventListener( 'click', function(e) {
            selectEntity( item );
        } );

        return cast item;
    }

    private function selectEntity(item) {
        if (m_selectedItem != null)
            m_selectedItem.classList.remove( 'selected' );

        m_selectedItem = item;

        item.classList.add( 'selected' );

        EntityInspector.instance.inspect( untyped item.entity );
    }
}
