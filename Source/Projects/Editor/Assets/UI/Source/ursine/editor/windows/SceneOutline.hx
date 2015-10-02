package ursine.editor.windows;

import js.html.UListElement;

class SceneOutline extends WindowHandler {
    private var m_entityList : UListElement;

    public function new() {
        super( );

        window.heading = "Outline";

        window.style.top = "50%";
        window.style.bottom = "0";
        window.style.left = "0";
        window.style.width = "15%";

        m_entityList = cast js.Browser.document.createElement( 'ul' );
        m_entityList.classList.add( 'entity-list' );

        window.container.appendChild( m_entityList );

        Editor.instance.broadcastManager.getChannel( 'EntityManager' ).on( 'EntityAdded', onEntityAdded );
    }

    private function onEntityAdded(e) {
        var item = js.Browser.document.createElement( 'li' );

        item.innerText = 'Entity ${e.uniqueID}';

        m_entityList.appendChild( item );
    }
}
