package ursine.api.ui;

import ursine.api.events.EventManager;
import ursine.api.events.IEventContainer;
import ursine.api.timers.TimerManager;

typedef ScreenID = UInt;

typedef ScreenConfig = {
    var project : String;
    var owner : ScreenManager;
    var id : ScreenID;
    var container : js.html.ShadowRoot;
    var data : Dynamic;
};

class Screen implements IEventContainer {
    public var events : EventManager;
    public var timers(default, null) : TimerManager;

    private static inline var m_pausedAnimationAttribute = '__ap';
    private static inline var m_baseScreenWidth : Int = 1280;
    private static inline var m_baseScreenHeight : Int = 720;

    private var m_project : String;
    private var m_owner : ScreenManager;
    private var m_id : ScreenID;
    private var m_container : js.html.ShadowRoot;

    private var m_data : Dynamic;

    private var m_paused : Bool;

    public function new(config : ScreenConfig) {
        events = new EventManager( );
        timers = new TimerManager( );

        m_project = config.project;
        m_owner = config.owner;
        m_id = config.id;
        m_container = config.container;

        m_data = config.data;

        m_paused = false;

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

    public function hasInputFocus() : Bool {
        return !m_paused && m_owner.hasInputFocus( this );
    }

    public function withInputFocus(callback : Dynamic) {
        return function() {
            if (hasInputFocus( ))
                callback( );
        };
    }

    public function addLocalScreen(path : String, initData : Dynamic = null, inputBlocking : Bool = false, priority : Int = 0) : ScreenID {
        return m_owner.addScreen( '${m_project}/${path}', initData, inputBlocking, priority );
    }

    public function exit() {
        m_owner.removeScreen( this );
    }

    public function pause() {
        if (m_paused)
            return;

        timers.pauseAll( );

        var elements = m_container.querySelectorAll( '*' );

        for (item in elements) {
            var element : js.html.Element = cast item;

            var state : String = element.style.animationPlayState;

            // if the value is empty, try to fetch it from the computed style
            if (state == '') {
                var style = js.Browser.window.getComputedStyle( element );

                state = style.getPropertyValue( 'animationPlayState' );
            }

            // determine if the computed animation state is running ("" implies running)
            if (state == '' || state == 'running') {
                element.setAttribute( m_pausedAnimationAttribute, 'true' );
                element.style.animationPlayState = 'paused';
            }
        }

        m_paused = true;
    }

    public function resume() {
        if (!m_paused)
            return;

        timers.resumeAll( );

        var elements = m_container.querySelectorAll( '[${m_pausedAnimationAttribute}]' );

        for (item in elements) {
            var element : js.html.Element = cast item;

            element.removeAttribute( m_pausedAnimationAttribute );
            element.style.animationPlayState = 'running';
        }

        m_paused = false;
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