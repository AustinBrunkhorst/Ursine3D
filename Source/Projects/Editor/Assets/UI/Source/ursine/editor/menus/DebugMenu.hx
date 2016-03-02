package ursine.editor.menus;

import ursine.native.Extern;

@menuIndex( 3 )
class DebugMenu extends MenuItemHandler {
    @mainMenuItem( "Debug/Editor UI/Reload" )
    static function doEditorReload() {
        js.Browser.location.reload( true );
    }

    @mainMenuItem( "Debug/Editor UI/Inspect" )
    static function doEditorDebugTools() {
        untyped __js__( "editor_commands_InspectEditorUI()" );
    }
}
