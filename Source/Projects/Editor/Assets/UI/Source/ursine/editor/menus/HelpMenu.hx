package ursine.editor.menus;

@menuIndex( 4 )
class HelpMenu extends MenuItemHandler {
    @mainMenuItem( "Help/Editor Documentation" )
    static function doOpenGettingStarted() {
        js.Browser.window.open( 'Documentation/html/index.html' );
    }
}
