import ursine.api.native.NativeBroadcastManager;

import ui.ScreenManager;

class Game {
    static function main() {
        var broadcastManager = new NativeBroadcastManager( );

        var screenManager = new ScreenManager(
            js.Browser.document.querySelector( '#screen-manager' )
        );
    }
}