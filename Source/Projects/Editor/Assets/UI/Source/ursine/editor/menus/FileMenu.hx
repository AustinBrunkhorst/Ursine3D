package ursine.editor.menus;

@menuIndex( 0 )
class FileMenu extends MenuItemHandler {
    @mainMenuItem( "File/New World", false, true )
    static function doNewWorld() {
        ursine.native.Extern.ProjectSetEmptyScene( );
    }

    @mainMenuItem( "File/Save World", false, false )
    static function doSaveWorld() {
        ursine.native.Extern.SceneSaveWorld( );
    }

    @mainMenuItem( "File/Save World As", false, false )
    static function doSaveWorldAs() {

    }

    @mainMenuItem( "File/Save Project", true, false )
    static function doSaveProject() {
        
    }
}