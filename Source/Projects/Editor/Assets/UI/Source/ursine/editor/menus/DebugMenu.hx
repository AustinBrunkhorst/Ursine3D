package ursine.editor.menus;

class DebugMenu extends MenuItemHandler {
    @mainMenuItem( "Debug/Debug Editor UI" )
    static function doEditorDebugTools() {
        untyped __js__( "DebugEditorUI( )" );
    }
}