package retrospect.screens;

import ursine.screen.Screen;

import ursine.input.KeyboardManager;
import ursine.input.KeyboardKey;

import ursine.input.GamepadManager;
import ursine.input.GamepadButton;

class MultiplayerPlayScreen extends Screen {
    public function new(id : ScreenID, frame : js.html.IFrameElement, data : Dynamic) {
        super( id, frame, data );

        events
            .on( KeyboardEventType.KeyDown, onKeyboardKeyDown )
            .on( GamepadEventType.ButtonDown, onGamepadButtonDown )
            .on( 'RaidStart', onRaidStart );
    }

    private function triggerPause() {
        Application.screenManager.addOverlay( 'PauseScreen', { } );
    }

    private function onKeyboardKeyDown(e : KeyboardKeyEvent) {
        if (!(e.triggered && e.pressed))
            return;

        if (e.key == KeyboardKey.ESCAPE) {
            triggerPause( );
        }
    }

    private function onGamepadButtonDown(e : GamepadButtonEvent) {
        if (!(e.triggered && e.pressed))
            return;

        if (e.button == GamepadButton.BACK) {
            triggerPause( );
        }
    }

    private function onRaidStart() {
        m_document.querySelector( '.player-hud' ).classList.add( 'active' );
    }
}