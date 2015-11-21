import ursine.native.Extern;

import ursine.input.GamepadManager;
import ursine.input.KeyboardManager;

import ursine.screen.ScreenManager;

class Application {
    static var gamepadManager : GamepadManager;
    static var keyboardManager : KeyboardManager;

    static function main() {
        gamepadManager = new GamepadManager( );
        keyboardManager = new KeyboardManager( );

        var screenManager = new ScreenManager( );

        Extern.InitGame( );
    }
}
