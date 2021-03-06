import ursine.editor.Editor;

import ursine.controls.EditorWindow;
import ursine.controls.docking.*;
import ursine.editor.windows.*;

class EditorMain {
    static function main() {
        js.Browser.window.addEventListener( 'WebComponentsReady', initWindows );
    }

    static private function initWindows() {
        var editor = new Editor( );

        var mainDockContainer =
            js.Browser.document.body.querySelector( '#main-dock-container' );

        editor.mainDock = new DockContainer( );

        editor.mainDock.style.width = '100%';
        editor.mainDock.style.height = '100%';

        mainDockContainer.appendChild( editor.mainDock );

        var sceneView = new SceneView( );
        var entityInspector = new EntityInspector( );
        var sceneOutline = new SceneOutline( );

        var projectBrowser = new ProjectBrowser( );

        var leftColumn = editor.mainDock.addColumn( );
        {
            leftColumn.style.width = '20%';

            var row = leftColumn.addRow( );

            row.style.height = '100%';

            var column = row.addColumn( );

            column.style.width = '100%';

            column.appendChild( entityInspector.window );
        }

        var rightColumn = editor.mainDock.addColumn( );
        {
            rightColumn.style.width = '80%';

            var topRow = rightColumn.addRow( );
            {
                topRow.style.height = '65%';

                var innerLeftColumn = topRow.addColumn( );
                {
                    innerLeftColumn.style.width = '80%';

                    var row = innerLeftColumn.addRow( );

                    row.style.height = '100%';

                    var column = row.addColumn( );

                    column.style.width = '100%';

                    column.appendChild( sceneView.window );
                }

                var innerRightColumn = topRow.addColumn( );
                {
                    innerRightColumn.style.width = '20%';

                    var row = innerRightColumn.addRow( );

                    row.style.height = '100%';

                    var column = row.addColumn( );

                    column.style.width = '100%';

                    column.appendChild( sceneOutline.window );
                }
            }

            var bottomRow = rightColumn.addRow( );
            {
                bottomRow.style.height = '35%';

                var column = bottomRow.addColumn( );
                {
                    column.style.width = '100%';

                    var row = column.addRow( );

                    row.style.height = '100%';

                    var column = row.addColumn( );

                    column.style.width = '100%';

                    column.appendChild( projectBrowser.window );
                }
            }
        }

        // TODO: remove after dock calls made
        haxe.Timer.delay( sceneView.onViewportInvalidated, 100 );
    }
}
