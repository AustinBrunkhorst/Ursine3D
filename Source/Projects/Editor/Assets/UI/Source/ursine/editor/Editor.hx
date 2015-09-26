package ursine.editor;

import ursine.controls.Menu;

class Editor {
    public static var instance : Editor = null;

    public var mainMenu : Menu;

    public function new() {
        instance = this;

        mainMenu = new Menu( );

        mainMenu.classList.add( 'main-menu' );

        js.Browser.document.querySelector( '#header-toolbar' ).appendChild( mainMenu );
    }
}
