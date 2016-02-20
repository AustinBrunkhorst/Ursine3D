package ursine.editor.windows;

import ursine.native.Extern;

class ProjectBrowser extends WindowHandler {
    var m_browser : ursine.controls.ProjectBrowser;

    public function new() {
        super( );

        window.heading = "Project";
        window.classList.add( 'project-browser-window' );

        m_browser = new ursine.controls.ProjectBrowser(
            Extern.ProjectGetResourceTree( )
        );

        window.container.appendChild( m_browser );
    }
}
