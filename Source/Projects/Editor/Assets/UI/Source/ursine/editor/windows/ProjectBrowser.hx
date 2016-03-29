package ursine.editor.windows;

import ursine.native.Extern;
import ursine.editor.resources.ResourceItem;

import ursine.controls.ContextMenu;
import ursine.controls.Notification;

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
            .on( 'ResourceModified', onResourceModified )
            .on( 'ResourceRemoved', onResourceRemoved )
            .on( 'ResourceRenamed', onResourceRenamed );
    }

    private function onResourceAdded(data : Dynamic) {
        var resource : ResourceItem = cast Extern.ProjectGetResource( data.guid );

        if (resource == null)
            throw 'Invalid resource added.';

        m_browser.addResource( resource );

        var notification = new Notification(
            NotificationType.Info,
            '<strong class="highlight">${resource.relativePathDisplayName}</div>',
            'Resource Imported'
        );

        notification.show( );
    }

    private function onResourceModified(data : Dynamic) {

    }

    private function onResourceRemoved(data : Dynamic) {
        var resource : ResourceItem = cast data.resource;

        m_browser.removeResource( resource );

        var notification = new Notification(
            NotificationType.Info,
            '<strong class="highlight">${resource.relativePathDisplayName}</div>',
            'Resource Deleted'
        );

        notification.show( );
    }

    private function onResourceRenamed(data : Dynamic) {
        var resource : ResourceItem = cast Extern.ProjectGetResource( data.guid );

        if (resource == null)
            throw 'Invalid resource renamed.';

        m_browser.renameResource( resource, data.oldName );

        var notification = new Notification(
            NotificationType.Info,
            '<strong class="highlight">${resource.relativePathDisplayName}</div>',
            'Resource Renamed'
        );

        notification.show( );
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
