import ursine.editor.Editor;

import ursine.controls.EditorWindow;
import ursine.controls.docking.*;
import ursine.editor.windows.*;

class Application {
    static function main() {
        js.Browser.window.addEventListener( 'load', function() {
            // TODO: figure out why there is an arbitrary delay
            haxe.Timer.delay( initWindows, 25 );
        } );
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
            leftColumn.style.width = '20%';

            var row = leftColumn.addRow( );

            row.setHeightPercent( 1.0 );

            var column = row.addColumn( );

            column.setWidthPercent( 1.0 );

            column.appendChild( new EntityInspector( ).window );
        }

        var middleColumn = mainDock.addColumn( );
        {
            middleColumn.style.width = '60%';

            var row = middleColumn.addRow( );

            row.setHeightPercent( 1.0 );

            var column = row.addColumn( );

            column.setWidthPercent( 1.0 );

            column.appendChild( sceneView.window );
        }

        var rightColumn = mainDock.addColumn( );
        {
            rightColumn.style.width = '20%';

            var row = rightColumn.addRow( );

            row.setHeightPercent( 1.0 );

            var column = row.addColumn( );

            column.setWidthPercent( 1.0 );

            column.appendChild( new SceneOutline( ).window );
        }

        // TODO: remove after dock calls made
        sceneView.onViewportInvalidated( );
    }
}
