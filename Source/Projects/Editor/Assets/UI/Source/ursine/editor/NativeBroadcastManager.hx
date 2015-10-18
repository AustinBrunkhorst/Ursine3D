package ursine.editor;

import ursine.native.Extern;
import ursine.utils.EventManager;

class NativeBroadcastManager {
    static private var m_instance : NativeBroadcastManager;

    private var m_channels : Map<String, EventManager> = new Map<String, EventManager>( );

    public function new() {
        m_instance = this;

        // hook our broadcast handler
        untyped js.Browser.window.NativeBroadcast = NativeBroadcastManager.onBroadcast;
    }

    public function getChannel(name : String) : EventManager {
        var channel = m_channels.get( name );

        if (channel == null) {
            channel = new EventManager( );

            m_channels.set( name, channel );
        }

        return channel;
    }

    private static function onBroadcast(target : String, message : String, data : Dynamic) {
        m_instance.getChannel( target ).trigger( message, data );
    }
}
