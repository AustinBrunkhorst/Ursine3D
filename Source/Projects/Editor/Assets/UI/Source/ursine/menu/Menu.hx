package ursine.menu;

import js.html.DOMElement;

class Menu implements IElementContainer {
    public var domContainer : DOMElement;

    public function new() {
        this.domContainer = js.Browser.document.createDivElement( );

        this.domContainer.classList.add( 'menu' );
    }
}
