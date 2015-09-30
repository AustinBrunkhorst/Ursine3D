package ursine.editor.menus;

@menuIndex( 0 )
class FileMenu extends MenuItemHandler {
    @mainMenuItem( "File/New" )
    static function doNew() {
        trace( "new" );
    }

    @mainMenuItem( "File/Open" )
    static function doOpen() {
        trace( "open" );
    }
}