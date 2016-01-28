package ursine.editor.menus;

@menuIndex( 4 )
class HelpMenu extends MenuItemHandler {
    @mainMenuItem( "Help/Getting Started" )
    static function doOpenGettingStarted() {
        js.Browser.window.open( 'Documentation/Getting Started/GettingStarted.html' );
    }
}
