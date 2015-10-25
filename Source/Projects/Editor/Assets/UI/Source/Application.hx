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

            var rows = [ leftColumn.addRow( ), leftColumn.addRow( ) ];

            for (row in rows) {
                row.addColumn( ).setWidthPercent( 1.0 );

                row.setHeightPercent( 0.5 );
            }

            var inspector = new EditorWindow( );

            inspector.heading = 'Inspector';

            rows[ 0 ].columns[ 0 ].appendChild( inspector );

            var outline = new EditorWindow( );

            outline.heading = 'Outline';

            rows[ 1 ].columns[ 0 ].appendChild( outline );
        }

        var middleColumn = mainDock.addColumn( );
        {
            middleColumn.setWidthPercent( 0.40 );

            var rows = [ middleColumn.addRow( ), middleColumn.addRow( ) ];

            for (row in rows) {
                row.addColumn( );

                row.setHeightPercent( 0.5 );
            }

            var content = new EditorWindow( );

            content.heading = 'Asset Browser';

            rows[ 0 ].columns[ 0 ].setWidthPercent( 1.0 );
            rows[ 0 ].columns[ 0 ].appendChild( content );

            var material = new EditorWindow( );

            material.heading = 'Material Editor';

            rows[ 1 ].columns[ 0 ].setWidthPercent( 0.5 );
            rows[ 1 ].columns[ 0 ].appendChild( material );

            var row2col2 = rows[ 1 ].addColumn( );

            var particle = new EditorWindow( );

            particle.heading = 'Particle Editor';

            row2col2.setWidthPercent( 0.5 );
            row2col2.appendChild( particle );
        }

        var rightColumn = mainDock.addColumn( );
        {
            rightColumn.setWidthPercent( 0.40 );

            var scene = new EditorWindow( );

            scene.heading = 'Scene';

            rightColumn.addRow( ).appendChild( scene );
        }
    }
}
