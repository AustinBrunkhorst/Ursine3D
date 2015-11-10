package ursine.editor.menus;

@menuIndex( 0 )
class FileMenu extends MenuItemHandler {
    @mainMenuItem( "File/Load Scene" )
    static function doNew() {
        ursine.native.Extern.SceneLoad( );
    }

    @mainMenuItem( "File/Save Scene" )
    static function doOpen() {
        ursine.native.Extern.SceneSave( );
    }
}