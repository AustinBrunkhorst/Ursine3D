package retrospect.screens;

import ursine.screen.Screen;

import ursine.utils.Utils;

import ursine.input.KeyboardManager;
import ursine.input.GamepadManager;

class MainMenuScreen extends BasicMenuScreen {
    public function new(id : ScreenID, frame : js.html.IFrameElement, data : Dynamic) {
        super( id, frame, data );

        handlers[ "play" ] = function() {
            transitionExit( function() {
                Application.screenManager.setScreen( 'MultiplayerPlayScreen', { } );
            } );
        };

        handlers[ "credits" ] = function() {

        };

        handlers[ "quit" ] = function() {
            Application.screenManager.addOverlay( 'ConfirmNavigationScreen', {
                title: 'Quit Game?',
                target: {
                    name: 'quit'
                }
            } );
        };
    }
}