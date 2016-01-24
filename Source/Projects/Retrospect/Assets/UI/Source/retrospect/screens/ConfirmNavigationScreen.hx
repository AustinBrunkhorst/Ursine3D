package retrospect.screens;

import ursine.screen.Screen;
import ursine.native.Extern;

class ConfirmNavigationScreen extends BasicMenuScreen {
    public function new(id : ScreenID, frame : js.html.IFrameElement, data : Dynamic) {
        super( id, frame, data );

        var title = m_document.querySelector( '#navigation-title' );

        title.innerHTML = data.title;

        handlers[ "yes" ] = function() {
            transitionExit( function() {
                if (data.target.name == 'quit') {
                    Extern.QuitGame( );
                }

                if (data.sender != null) {
                    var sender = Application.screenManager.getScreen( data.sender );

                    if (sender != null)
                        sender.exit( );
                }

                if (data.removeCurrent != null) {
                    Application.screenManager.removeCurrent( );
                }

                var outputData = data.target.data == null ? { } : data.target.data;

                if (data.target.type == "screen") {
                    Application.screenManager.setScreen(
                        data.target.name,
                        outputData
                    );
                } else {
                    Application.screenManager.addOverlay(
                        data.target.name,
                        outputData
                    );
                }
            }, 'bounceOut' );
        };

        handlers[ "no" ] = function() {
            transitionExit( null, 'bounceOut' );
        };
    }
}