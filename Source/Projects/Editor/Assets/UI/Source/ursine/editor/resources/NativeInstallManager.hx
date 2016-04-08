package ursine.editor.resources;

import ursine.native.Extern;
import ursine.api.native.NativeBroadcastManager;

import ursine.controls.Notification;
import ursine.controls.MenuItem;

import ursine.editor.windows.OutputLog;

class NativeInstallManager {
    private var m_menuInstallItem : MenuItem;
    private var m_progressNotification : Notification;

    private var m_lastStartDate : Date;

    public function new(broadcastManager : NativeBroadcastManager) {
        m_menuInstallItem = Editor.instance.mainMenu
            .findItem( 'Project' ).menu
            .findItem( 'Build Installer' );

        broadcastManager.getChannel( 'ResourcePipeline' )
            .on( 'ProjectInstallStart', onInstallStart )
            .on( 'ProjectInstallComplete', onInstallComplete );
    }

    private function onInstallStart(e) {
        m_menuInstallItem.disabled = true;

        m_lastStartDate = Date.now( );

        if (m_progressNotification != null) {
            m_progressNotification.close( );

            m_progressNotification = null;
        }

        m_progressNotification = new Notification(
            NotificationType.Loading,
            'This may take a while.',
            'Building Installer'
        );

        m_progressNotification.dismissible = false;

        m_progressNotification.show( 0 );
    }

    private function onInstallComplete(e) {
        m_menuInstallItem.disabled = false;

        m_progressNotification.close( );
        m_progressNotification = null;

        var notification : Notification;

        if (e.successful) {
            notification = new Notification(
                NotificationType.Info,
                '',
                'Install Build Successful'
            );

            var duration = Date.now( ).getTime( ) - m_lastStartDate.getTime( );

            OutputLog.log( 'Install Build Completed in ${duration} ms' );
        } else {
            notification = new Notification(
                NotificationType.Error,
                'Check output console for more info.',
                'Install Build Failed'
            );

            OutputLog.log( 'Install Build Failed: ${e.error}' );
        }

        notification.show( );
    }
}