package retrospect.screens;

import ursine.screen.Screen;

import ursine.utils.Utils;

import ursine.input.KeyboardManager;
import ursine.input.GamepadManager;

class HowToPlayScreen extends Screen {
    private var m_leaving : Bool;
    private var m_controls : js.html.DivElement;

    public function new(id : ScreenID, frame : js.html.IFrameElement, data : Dynamic) {
        super( id, frame, data );

        m_leaving = false;
        m_controls = cast m_document.querySelector( '#controls' );

        m_document.addEventListener( 'mousedown', leave );

        events
            .on( KeyboardEventType.KeyDown, leave )
            .on( GamepadEventType.ButtonDown, leave );
    }

    function leave() {
        if (m_leaving)
            return;

        m_leaving = true;

        m_controls.className = 'animated zoomOut';

        ElementUtils.once( m_controls, 'webkitAnimationEnd', exit );
    }
}