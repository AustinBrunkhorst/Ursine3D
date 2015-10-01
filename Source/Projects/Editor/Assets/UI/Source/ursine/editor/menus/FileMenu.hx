package ursine.editor.menus;

@menuIndex( 0 )
class FileMenu extends MenuItemHandler {
    @mainMenuItem( "File/New" )
    static function doNew() {
        trace( "NativeEditorHandler" );
    }

    @mainMenuItem( "File/Open" )
    static function doOpen() {
        trace( "open" );
    }
}