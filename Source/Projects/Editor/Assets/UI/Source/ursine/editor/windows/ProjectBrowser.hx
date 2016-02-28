package ursine.editor.windows;

import ursine.controls.ContextMenu;
import ursine.native.Extern;

extern class ResourceItem {
    var guid : String;
    var type : String;
    var displayName : String;
    var relativePathDisplayName : String;
    var sourceFile : String;
    var hasPreview : Bool;
    var previewFile : String;
    var extension : String;
}

class ProjectBrowser extends WindowHandler {
    var m_browser : ursine.controls.ProjectBrowser;

    public function new() {
        super( );

        window.heading = "Project";
        window.classList.add( 'project-browser-window' );

        m_browser = new ursine.controls.ProjectBrowser(
            Extern.ProjectGetResourceTree( )
        );

        m_browser.addEventListener( 'resource-dblclick', onResourceDblClick );
        m_browser.addEventListener( 'resource-contextmenu', onResourceContextMenu );

        window.container.appendChild( m_browser );

        Editor.instance.broadcastManager.getChannel( 'ResourcePipeline' )
            .on( 'ResourceAdded', onResourceAdded )
            .on( 'ResourceModified', onResourceModified );
    }

    private function onResourceAdded(data : Dynamic) {
        trace( 'added ${data.guid}' );
    }

    private function onResourceModified(data : Dynamic) {
        trace( 'modified ${data.guid}' );
    }

    private function onResourceDblClick(e : js.html.CustomEvent) {
        // skip over dot
        var extension = e.detail.resource.extension.substr( 1 ).toLowerCase( );

        var handler = Reflect.field( this, '${extension}_DblClickHandler' );

        if (handler != null)
            handler( e );
    }

    private function onResourceContextMenu(e : js.html.CustomEvent) {
        var resource : ResourceItem = e.detail.resource;

        // skip over dot
        var extension = resource.extension.substr( 1 ).toLowerCase( );

        var handler = Reflect.field( this, '${extension}_ContextMenuHandler' );

        if (handler != null) {
            e.detail.menu.addSeparator( );

            handler( e );
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    // Double Click Handlers
    ///////////////////////////////////////////////////////////////////////////

    @:keep
    public function uworld_DblClickHandler(e : js.html.CustomEvent) {
        var resource : ResourceItem = e.detail.resource;

        Extern.SceneSetActiveWorld( resource.guid );
    }

    ///////////////////////////////////////////////////////////////////////////
    // Context Menu Handlers
    ///////////////////////////////////////////////////////////////////////////

}
