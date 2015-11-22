package ursine.screen;

import ursine.utils.IEventContainer;
import ursine.utils.EventManager;

typedef ScreenID = UInt;

@:keepInit
@:keepSub
class Screen implements IEventContainer {
    public var events : EventManager;

    private var m_id : ScreenID;
    private var m_frame : js.html.IFrameElement;
    private var m_document : js.html.HTMLDocument;

    private var m_data : Dynamic;

    public function new(id : ScreenID, frame : js.html.IFrameElement, data : Dynamic) {
        m_id = id;
        m_frame = frame;
        m_document = frame.contentDocument;

        m_data = data;
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
}