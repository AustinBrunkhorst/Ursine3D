import ursine.editor.Editor;

import ursine.controls.EditorWindow;
import ursine.controls.docking.*;
import ursine.editor.windows.*;

class Application {
    static function main() {
        js.Browser.window.addEventListener( 'WebComponentsReady', initWindows );
    }

    static private function initWindows() {
        var editor = new Editor( );

        var mainDockContainer =
            js.Browser.document.body.querySelector( '#main-dock-container' );

        var mainDock = new DockContainer( );

        mainDock.style.width = '100%';
        mainDock.style.height = '100%';

        mainDockContainer.appendChild( mainDock );

        var sceneView = new SceneView( );

        var leftColumn = mainDock.addColumn( );
        {
            leftColumn.style.width = '15%';

            var row = leftColumn.addRow( );

            row.style.height = '100%';

            var column = row.addColumn( );

            column.style.width = '100%';

            column.appendChild( new EntityInspector( ).window );
        }

        var middleColumn = mainDock.addColumn( );
        {
            middleColumn.style.width = '70%';

            var row = middleColumn.addRow( );

            row.style.height = '100%';

            var column = row.addColumn( );

            column.style.width = '100%';

            column.appendChild( sceneView.window );
        }

        var rightColumn = mainDock.addColumn( );
        {
            rightColumn.style.width = '15%';

            var row = rightColumn.addRow( );

            row.style.height = '100%';

            var column = row.addColumn( );

            column.style.width = '100%';

            column.appendChild( new SceneOutline( ).window );
        }

        // TODO: remove after dock calls made
        sceneView.onViewportInvalidated( );
    }
}
