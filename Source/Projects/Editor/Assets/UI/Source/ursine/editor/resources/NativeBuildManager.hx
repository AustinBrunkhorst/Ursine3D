package ursine.editor.resources;

import ursine.native.Extern;
import ursine.api.native.NativeBroadcastManager;

import ursine.controls.Button;
import ursine.controls.Notification;
import ursine.controls.ProgressBar;
import ursine.controls.MenuItem;

import ursine.editor.resources.NativeResourceManager.ResourceBuildEventArgs;

import ursine.editor.windows.OutputLog;

class NativeBuildManager {
    private var m_menuBuildItem : MenuItem;

    public function new(broadcastManager : NativeBroadcastManager) {
        m_menuBuildItem = Editor.instance.mainMenu
            .findItem( 'Project' ).menu
            .findItem( 'Build' );

        broadcastManager.getChannel( 'ResourcePipeline' )
            .on( 'ProjectBuildStart', onResourceBuildStart )
            .on( 'ProjectBuildProgress', onResourceBuildProgress )
            .on( 'ProjectBuildComplete', onResourceBuildComplete );

        var progress = new Notification(
            NotificationType.Info,
            '',
            'Building Game'
        );

        progress.dismissible = false;

        var progressBar = new ProgressBar( );

        progressBar.value = 50;

        progress.messageContainer.appendChild( progressBar );

        var btnCancel = new Button( );
        {
            btnCancel.text = 'Cancel';

            btnCancel.addEventListener( 'click', function() {
                m_menuBuildItem.disabled = false;

                progress.close( );
            } );
        }

        progress.buttonsContainer.appendChild( btnCancel );

        progress.show( 0 );
    }

    private function onResourceBuildStart(e : ResourceBuildEventArgs) {
        var resource : ResourceItem = Extern.ProjectGetResource( e.guid );

        if (resource == null)
            throw 'Failed to get resource from GUID ${e.guid}';

        OutputLog.log( 'Building ${resource.relativePathDisplayName}...' );
    }

    private function onResourceBuildProgress(e) {

    }

    private function onResourceBuildComplete(e : ResourceBuildEventArgs) {
        var resource : ResourceItem = Extern.ProjectGetResource( e.guid );

        if (resource == null)
            throw 'Failed to get resource from GUID ${e.guid}';

        if (e.successful) {
            OutputLog.log( 'Built in ${e.duration} ms: ${resource.relativePathDisplayName}' );
        } else {
            OutputLog.log( 'Build Failed: ${resource.relativePathDisplayName}<br>Reason: ${e.error.message}' );
        }
    }
}