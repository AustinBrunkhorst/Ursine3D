package ursine;

import ursine.menu.MainMenu;

class Editor {
    public static var instance : Editor = null;

    public var mainMenu : MainMenu;

    public function new() {
        instance = this;

        mainMenu = new MainMenu( );
    }
}
