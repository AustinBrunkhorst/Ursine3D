package ursine.editor.menus;

import ursine.editor.windows.SceneView;

@menuIndex( 2 )
class ViewMenu extends MenuItemHandler {
    @mainMenuItem( "View/Fullscreen Scene" )
    static function doFullscreenToggle() {
        SceneView.instance.toggleFullscreen( );
    }
}
