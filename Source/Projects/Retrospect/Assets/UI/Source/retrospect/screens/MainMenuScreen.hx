package retrospect.screens;

import ursine.screen.Screen;

import ursine.utils.Utils;

import ursine.input.KeyboardManager;
import ursine.input.GamepadManager;

class MainMenuScreen extends Screen {
    public function new(id : ScreenID, frame : js.html.IFrameElement, data : Dynamic) {
        super( id, frame, data );

        m_document.addEventListener( 'mousedown', play );

        events
            .on( KeyboardEventType.KeyDown, play )
            .on( GamepadEventType.ButtonDown, play );
    }

    public override function exit() {
        super.exit( );

        Application.screenManager.setScreen( 'MultiplayerPlayScreen', { } );
    }

    private function play() {
    	var header = m_document.querySelector( 'header' );

    	header.className = 'animated fadeOutUp';

        ElementUtils.once( header, 'webkitAnimationEnd', exit );
    }
}