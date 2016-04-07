package ursine.editor.resources;

import ursine.native.Extern;
import ursine.api.native.NativeBroadcastManager;

import ursine.controls.Button;
import ursine.controls.Notification;
import ursine.controls.ProgressBar;
import ursine.controls.MenuItem;

import ursine.editor.windows.OutputLog;

class NativeBuildManager {
    private var m_menuBuildItem : MenuItem;
    private var m_progressNotification : Notification;
    private var m_progressBar : ProgressBar;

    private var m_lastStartDate : Date;

    public function new(broadcastManager : NativeBroadcastManager) {
        m_menuBuildItem = Editor.instance.mainMenu
            .findItem( 'Project' ).menu
            .findItem( 'Build' );

        broadcastManager.getChannel( 'ResourcePipeline' )
            .on( 'ProjectBuildStart', onResourceBuildStart )
            .on( 'ProjectBuildProgress', onResourceBuildProgress )
            .on( 'ProjectBuildComplete', onResourceBuildComplete );
    }

    private function onResourceBuildStart(e) {
        m_menuBuildItem.disabled = true;

        m_lastStartDate = Date.now( );

        if (m_progressNotification != null) {
            m_progressNotification.close( );

            m_progressNotification = null;
        }

        m_progressNotification = new Notification(
            NotificationType.Info,
            '',
            'Building Game'
        );

        m_progressNotification.dismissible = false;

        m_progressBar = new ProgressBar( );

        m_progressNotification.messageContainer.appendChild( m_progressBar );

        var btnCancel = new Button( );
        {
            btnCancel.text = 'Cancel';

            btnCancel.addEventListener( 'click', function() {
                Extern.ProjectBuildCancel( );
            } );
        }

        m_progressNotification.buttonsContainer.appendChild( btnCancel );

        m_progressNotification.show( 0 );
    }

    private function onResourceBuildProgress(e) {
        if (m_progressBar != null)
            m_progressBar.value = e.progress * 100;
    }

    private function onResourceBuildComplete(e) {
        m_menuBuildItem.disabled = false;

        m_progressNotification.close( );
        m_progressNotification = null;
        m_progressBar = null;

        var notification : Notification;

        if (e.successful) {
            notification = new Notification(
                NotificationType.Info,
                '',
                'Build Successful'
            );

            var duration = Date.now( ).getTime( ) - m_lastStartDate.getTime( );

            OutputLog.log( 'Build Completed in ${duration} ms' );
        } else {
            notification = new Notification(
                NotificationType.Error,
                'Check output console for more info.',
                'Build Failed'
            );

            OutputLog.log( 'Build Failed: ${e.error}' );
        }

        notification.show( );
    }
}