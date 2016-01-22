import ursine.native.Extern;

import ursine.utils.NativeBroadcastManager;

import ursine.input.GamepadManager;
import ursine.input.KeyboardManager;

import ursine.screen.ScreenManager;

// ensure DCE doesn't ignore the screens
import retrospect.screens.Screens;

class Application {
    public static var broadcastManager : NativeBroadcastManager;

    public static var gamepadManager : GamepadManager;
    public static var keyboardManager : KeyboardManager;

    public static var screenManager : ScreenManager;

    static function main() {
        broadcastManager = new NativeBroadcastManager( );
        gamepadManager = new GamepadManager( );
        keyboardManager = new KeyboardManager( );

        screenManager = new ScreenManager( 'retrospect.screens.' );

        Extern.InitGame( );
    }
}
