package ursine.editor.menus;

@menuIndex( 0 )
class FileMenu extends MenuItemHandler {
    @mainMenuItem( "File/Load Scene", false, false, "file-open" )
    static function doNew() {
        ursine.native.Extern.SceneLoad( );
    }

    @mainMenuItem( "File/Save Scene", false, false, "file-save" )
    static function doOpen() {
        ursine.native.Extern.SceneSave( );
    }
}