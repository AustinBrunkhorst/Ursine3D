package retrospect.screens;

import ursine.screen.Screen;
import ursine.native.Extern;

import ursine.utils.Utils;

import ursine.input.KeyboardManager;
import ursine.input.GamepadManager;

class MainMenuScreen extends BasicMenuScreen {
    private static var m_mainMenuMusic = "MUSIC_THEME";

    public function new(id : ScreenID, frame : js.html.IFrameElement, data : Dynamic) {
        super( id, frame, data );

        handlers[ "play" ] = function() {
            transitionExit( function() {
                Application.screenManager.setScreen( 'MultiplayerPlayScreen', { } );
            } );
        };

        handlers[ "how-to-play" ] = function() {
            Application.screenManager.addOverlay( 'HowToPlayScreen', { } );
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

        var musicPlaying = Extern.AudioIsGlobalEventPlaying( m_mainMenuMusic );

        if (!musicPlaying) {
            Extern.AudioPlayGlobalEvent( m_mainMenuMusic );
        }
    }

    public override function exit() {
        super.exit( );

        var musicPlaying = Extern.AudioIsGlobalEventPlaying( m_mainMenuMusic );

        if (musicPlaying) {
            Extern.AudioStopGlobalEvent( m_mainMenuMusic );
        }
    }
}