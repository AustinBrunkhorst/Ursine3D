package ursine.api.ui;

import ursine.api.events.EventManager;
import ursine.api.events.IEventContainer;

typedef ScreenID = UInt;

class Screen implements IEventContainer {
    public var events : EventManager;

    private static inline var m_baseScreenWidth : Int = 1280;
    private static inline var m_baseScreenHeight : Int = 720;

    private var m_id : ScreenID;
    private var m_container : js.html.ShadowRoot;

    private var m_data : Dynamic;

    public function new(id : ScreenID, container : js.html.ShadowRoot, data : Dynamic) {
        events = new EventManager( );

        m_id = id;
        m_container = container;

        m_data = data;

        invalidateViewport( );
    }

    public function getID() : ScreenID {
        return m_id;
    }

    public function getHost() : js.html.Element {
        return m_container.host;
    }

    public function getContainer() : js.html.ShadowRoot {
        return m_container;
    }

    public function exit() {
        events.trigger( '.exit', null );
    }

    public function invalidateViewport() {
        var aspectContainer = m_container.querySelector( '.aspect-ratio-container' );

        if (aspectContainer == null)
            return;

        var host = getHost( );

        untyped aspectContainer.style.zoom = Math.min(
            host.clientWidth / m_baseScreenWidth,
            host.clientHeight / m_baseScreenHeight
        );
    }
}