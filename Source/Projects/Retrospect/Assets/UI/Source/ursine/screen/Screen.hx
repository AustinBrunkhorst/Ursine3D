package ursine.screen;

import retrospect.screens.PauseScreen;
import ursine.utils.IEventContainer;
import ursine.utils.EventManager;

typedef ScreenID = UInt;

@:keepInit
@:keepSub
class Screen implements IEventContainer {
    public var events : EventManager;

    private static var m_baseScreenWidth : Int = 1280;
    private static var m_baseScreenHeight : Int = 720;

    private var m_id : ScreenID;
    private var m_frame : js.html.IFrameElement;
    private var m_document : js.html.HTMLDocument;

    private var m_data : Dynamic;

    public function new(id : ScreenID, frame : js.html.IFrameElement, data : Dynamic) {
        events = new EventManager( );

        m_id = id;
        m_frame = frame;
        m_document = frame.contentDocument;

        m_data = data;

        js.Browser.window.addEventListener( 'resize', handleAspectResize );

        handleAspectResize( );
    }

    public function getID() : ScreenID {
        return m_id;
    }

    public function getFrame() : js.html.IFrameElement {
        return m_frame;
    }

    public function exit() {
        Application.screenManager.removeScreen( this );
    }

    private function handleAspectResize() {
        var aspectContainer = m_document.querySelector( '.aspect-ratio-container' );

        if (aspectContainer == null)
            return;

        untyped aspectContainer.style.zoom = Math.min(
            js.Browser.window.innerWidth / m_baseScreenWidth,
            js.Browser.window.innerHeight / m_baseScreenHeight
        );
    }
}