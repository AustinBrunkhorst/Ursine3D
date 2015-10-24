import ursine.editor.Editor;

import ursine.editor.WindowHandler;

import ursine.editor.windows.*;

class Application {
    static function main() {
        var editor = new Editor( );

        editor.broadcastManager.getChannel( 'EntityManager' ).on( 'EntityCreated', function(data) {
            trace( data );
        } );

        var windows : Array<WindowHandler> = [
            new EntityInspector( ),
            new SceneOutline( ),
            new SceneView( )
        ];

        var windowContainer = js.Browser.document.body.querySelector( '#window-container' );

        for (handler in windows)
            windowContainer.appendChild( handler.window );
    }
}
