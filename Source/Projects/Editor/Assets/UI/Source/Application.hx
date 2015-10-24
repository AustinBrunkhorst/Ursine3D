import ursine.editor.Editor;

import ursine.editor.WindowHandler;

import ursine.controls.EditorWindow;
import ursine.controls.docking.*;
import ursine.editor.windows.*;

class Application {
    static function main() {
        //var editor = new Editor( );

        var mainDockContainer =
            js.Browser.document.body.querySelector( '#main-dock-container' );

        var mainDock = new DockContainer( );

        mainDock.style.width = '100%';
        mainDock.style.height = '100%';

        mainDockContainer.appendChild( mainDock );

        var leftColumn = mainDock.addColumn( );
        {
            leftColumn.setWidthPercent( 0.20 );

            var inspector = new EditorWindow( );

            inspector.heading = 'Inspector';

            leftColumn.addNode( inspector );

            var outline = new EditorWindow( );

            outline.heading = 'Outline';

            leftColumn.addNode( outline );
        }

        var middleColumn = mainDock.addColumn( );
        {
            middleColumn.setWidthPercent( 0.40 );

            var content = new EditorWindow( );

            content.heading = 'Content Browser';

            middleColumn.addNode( content );
        }

        var rightColumn = mainDock.addColumn( );
        {
            rightColumn.setWidthPercent( 0.40 );

            var scene = new EditorWindow( );

            scene.heading = 'Scene';

            rightColumn.addNode( scene );
        }
    }
}
