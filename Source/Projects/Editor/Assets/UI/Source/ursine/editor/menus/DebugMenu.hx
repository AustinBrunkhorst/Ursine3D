package ursine.editor.menus;

import ursine.native.Extern;

class DebugMenu extends MenuItemHandler {
    @mainMenuItem( "Debug/Editor UI/Reload" )
    static function doEditorReload() {
        // TODO: fix javascript context issues after reload
        //js.Browser.location.reload( true );
    }

    @mainMenuItem( "Debug/Editor UI/Inspect" )
    static function doEditorDebugTools() {
        Extern.DebugEditorUI( );
    }
}