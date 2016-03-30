package ursine.editor.menus;

import ursine.native.Extern;

@menuIndex( 7 )
class HelpMenu extends MenuItemHandler {
    @mainMenuItem( "Help/Editor Documentation" )
    static function doOpenGettingStarted() {
        Extern.ProcessOpen( 'Assets/UI/Resources/Documentation/html/index.html', true );
    }
}
