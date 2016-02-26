package ursine.editor.menus;

import ursine.controls.Notification;

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
        ursine.native.Extern.SceneSaveWorldAs( );
    }

    @mainMenuItem( "File/Save Project", true, false )
    static function doSaveProject() {
        var notification = new Notification(
            NotificationType.Info,
            "Thanks for saving fella, but this doesn't do anything right now.",
            "Save Project"
        );

        notification.show( );
    }
}