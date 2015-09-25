package ursine.menu;

import js.html.DOMElement;

import ursine.utils.*;

class MenuItem implements IElementContainer implements IEventContainer {
    public var domContainer : DOMElement;
    public var events : EventManager = new EventManager( );

    public var subMenu : Menu = new Menu( );

    public var text(get, set) : String;

    public function get_text() return domContainer.innerHTML;
    public function set_text(text) return domContainer.innerHTML = text;

    public function new(text) {
        domContainer = HTMLTemplate.load( "Templates/Menu/MenuItem.html" ).compile( );

        domContainer.addEventListener( 'click', function(e) {
            events.trigger( 'click', e );
        } );

        this.text = text;
    }
}
