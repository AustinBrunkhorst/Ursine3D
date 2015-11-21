package ursine.screen;

import ursine.utils.IEventContainer;
import ursine.utils.EventManager;

typedef ScreenID = UInt;

class Screen implements IEventContainer {
    public var events : EventManager;

    private var m_id : ScreenID;
    private var m_document : js.html.Document;

    public function new() {

    }

    public function exit() {
        ScreenManager.instance.removeScreen( this );
    }
}