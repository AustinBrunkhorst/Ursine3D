package ursine.screen;

import ursine.screen.Screen;

class ScreenManager {
    public static var instance : ScreenManager;

    private var m_screens : Map<ScreenID, Screen>;
    private var m_screensContainer : js.html.Element;

    public function new() {
        instance = this;

        m_screens = new Map<ScreenID, Screen>( );
        m_screensContainer = js.Browser.document.querySelector( '#screens' );
    }

    public function removeScreen(screen : Screen) {
        // TODO:
    }
}