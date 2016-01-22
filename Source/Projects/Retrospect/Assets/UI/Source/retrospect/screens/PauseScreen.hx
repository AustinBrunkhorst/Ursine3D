package retrospect.screens;

import ursine.screen.Screen;

class PauseScreen extends BasicMenuScreen {
    public function new(id : ScreenID, frame : js.html.IFrameElement, data : Dynamic) {
        super( id, frame, data );

        handlers[ "resume" ] = function() {
            transitionExit( null, 'bounceOut' );
        };

        handlers[ "restart" ] = function() {
            Application.screenManager.addOverlay( 'ConfirmNavigationScreen', {
                title: 'Restart?',
                sender: getID( ),
                target: {
                    name: 'MultiplayerPlayScreen',
                    type: 'screen'
                },
                removeCurrent: true
            } );
        };

        handlers[ "main-menu" ] = function() {
            Application.screenManager.addOverlay( 'ConfirmNavigationScreen', {
                title: 'Return to Main Menu?',
                sender: getID( ),
                target: {
                    name: 'MainMenuScreen',
                    type: 'overlay'
                },
                removeCurrent: true
            } );
        };

        handlers[ "how-to-play" ] = function() {
            Application.screenManager.addOverlay( 'HowToPlayScreen', { } );
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