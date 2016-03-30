package ursine.editor;

import ursine.native.Extern;

import ursine.controls.Button;
import ursine.controls.Notification;

class NativeNotificationManager {
    private var m_notifications : Map<UInt, Notification>;

    public function new(broadcastManager : NativeBroadcastManager) {
        m_notifications = new Map<UInt, Notification>( );

        broadcastManager.getChannel( 'EditorNotification' )
            .on( 'created', onCreated )
            .on( 'close', onCloseRequested );
    }

    private function onCreated(e : Dynamic) {
        var notification = new Notification( e.type, e.message, e.header );

        notification.mode = e.mode;

        notification.dismissible = e.dismissible;

        initNotificationButtons( e.id, notification, e.buttons );

        notification.addEventListener( 'closed', function() {
            Extern.NotificationCloseCallback( e.id );

            m_notifications.remove( e.id );
        } );

        notification.show( e.duration );

        m_notifications[ e.id ] = notification;
    }

    private function onCloseRequested(e : Dynamic) {
        var notification = m_notifications[ e.id ];

        if (notification == null)
            throw 'Invalid notification close requested: ${e.id}';

        notification.close( );
    }

    private function initNotificationButtons(id : UInt, notification : Notification, buttons : Array<Dynamic>) {
        var i = 0;

        for (button in buttons) {
            var element = new Button( );

            element.text = button.text;

            element.addEventListener( 'click', function(index) {
                Extern.NotificationButtonCallback( id, index );
            }.bind( i ) );

            notification.buttonsContainer.appendChild( element );

            ++i;
        }
    }
}
