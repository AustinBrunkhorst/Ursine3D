package ursine.editor;

import ursine.controls.MainMenu;

class Editor {
    public static var instance : Editor = null;

    public var mainMenu : MainMenu;

    public function new() {
        instance = this;

        mainMenu = new MainMenu( );

        js.Browser.document.querySelector( '#header-toolbar' ).appendChild( mainMenu );
    }
}
