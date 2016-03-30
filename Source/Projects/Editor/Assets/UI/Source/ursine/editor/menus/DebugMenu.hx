package ursine.editor.menus;

import ursine.native.Extern;

import ursine.controls.Notification;
import ursine.editor.scene.ScenePlayState;

@menuIndex( 4 )
class DebugMenu extends MenuItemHandler {
    @mainMenuItem( "Debug/Editor UI/Reload" )
    static function doEditorReload() {
        untyped __js__( "editor_commands_ReloadEditorUI()" );
    }

    @mainMenuItem( "Debug/Editor UI/Inspect" )
    static function doEditorDebugTools() {
        untyped __js__( "editor_commands_InspectEditorUI()" );
    }
}
