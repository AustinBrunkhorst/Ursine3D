package ursine.editor.menus;

@menuIndex( 0 )
class DebugMenu extends MenuItemHandler {
    @mainMenuItem( "Debug/Debug Editor UI" )
    static function doEditorDebugTools() {
        trace( "debug" );
    }
}