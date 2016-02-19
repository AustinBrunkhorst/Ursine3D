package ursine.editor.windows;

import ursine.controls.TreeView;
import ursine.controls.TreeViewItem;

class ProjectBrowser extends WindowHandler {
    var m_explorer : TreeView;

    public function new() {
        super( );

        window.heading = "Project";
        window.classList.add( 'project-browser-window' );

        initElements( );
        initExplorer( );
    }

    private function initElements() {
        window.container.innerHTML = '
            <div class="columns">
                <div class="left"></div>
                <div class="right>
                    <div class="bread-crumbs"></div>
                    <div class="folder-contents"></div>
                </div>
            </div>
        ';
    }

    private function initExplorer() {
        m_explorer = new TreeView( );
        {
            m_explorer.setAsRoot( true );
            m_explorer.enableModification = false;

            m_explorer.classList.add( 'explorer' );
        }

        var item = new TreeViewItem( );

        item.text = 'Textures';

        m_explorer.appendChild( item );

        window.container.querySelector( '.columns .left' ).appendChild( m_explorer );
    }
}
