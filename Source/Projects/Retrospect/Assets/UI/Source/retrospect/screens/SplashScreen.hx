package retrospect.screens;

import ursine.screen.Screen;

import ursine.utils.Utils;

import ursine.input.KeyboardManager;
import ursine.input.GamepadManager;

class SplashScreen extends Screen {
    public function new(id : ScreenID, frame : js.html.IFrameElement, data : Dynamic) {
        super( id, frame, data );

        var logo = m_document.querySelector( '#logo' );

        // transition in
        ElementUtils.once( logo, 'webkitAnimationEnd', function() {
            haxe.Timer.delay( function() {
                logo.className = 'animated zoomOutUp';
            }, 1000 );

            // transition out
            ElementUtils.once( logo, 'webkitAnimationEnd', exit );
        } );

        m_document.addEventListener( 'mousedown', exit );

        events
            .on( KeyboardEventType.KeyDown, exit )
            .on( GamepadEventType.ButtonDown, exit );
    }

    public override function exit() {
        super.exit( );

        Application.screenManager.addOverlay( 'MainMenuScreen', { } );
    }
}