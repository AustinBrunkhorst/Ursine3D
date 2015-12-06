package retrospect.screens;

import ursine.screen.Screen;
import ursine.utils.Utils;

class RoundOverlayScreen extends Screen {
    private var m_targetScreen : String;

    public function new(id : ScreenID, frame : js.html.IFrameElement, data : Dynamic) {
        super( id, frame, data );

        m_targetScreen = data.targetScreen;

        var title = m_document.querySelector( '.title' );

        title.innerHTML = data.title;

        // transition in
        ElementUtils.once( title, 'webkitAnimationEnd', function() {
            haxe.Timer.delay( function() {
                title.className = 'title animated zoomOut';
            }, 1000 );

            // transition out
            ElementUtils.once( title, 'webkitAnimationEnd', exit );
        } );
    }

    public override function exit() {
        super.exit( );

        if (m_targetScreen != null) {
            Application.screenManager.removeCurrent( );
            Application.screenManager.addOverlay( m_targetScreen, { } );
        }
    }
}