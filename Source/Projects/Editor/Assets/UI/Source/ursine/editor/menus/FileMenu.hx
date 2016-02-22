package ursine.editor.menus;

@menuIndex( 0 )
class FileMenu extends MenuItemHandler {
    @mainMenuItem( "File/New World", false, true )
    static function doNewWorld() {
        ursine.native.Extern.SceneCreateEmptyWorld( );
    }

    @mainMenuItem( "File/Save World", false, false )
    static function doSaveWorld() {
        ursine.native.Extern.SceneSaveWorld( );
    }
}