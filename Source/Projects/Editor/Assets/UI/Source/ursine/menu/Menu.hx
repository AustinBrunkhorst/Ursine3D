package ursine.menu;

import js.html.DOMElement;

import ursine.utils.HTMLTemplate;

class Menu implements IElementContainer {
    public var domContainer : DOMElement;

    public function new() {
        //domContainer = HTMLTemplate.load( "Templates/Menu/Menu.html" ).compile( );
    }

    public function addItem(item : MenuItem) : Void {
        domContainer.appendChild( item.domContainer );
    }

    public function removeItem(item : MenuItem) : Void {
        domContainer.removeChild( item.domContainer );
    }
}