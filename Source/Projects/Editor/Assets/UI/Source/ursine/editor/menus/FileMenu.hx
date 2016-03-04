package ursine.editor.menus;

import ursine.native.Extern;

import ursine.controls.Notification;

@menuIndex( 0 )
class FileMenu extends MenuItemHandler {
    @mainMenuItem( "File/New World", false, true )
    static function doNewWorld() {
        Extern.ProjectSetEmptyScene( );
    }

    @mainMenuItem( "File/Save World", false, false )
    static function doSaveWorld() {
        Extern.SceneSaveWorld( );
    }

    @mainMenuItem( "File/Save World As", false, false )
    static function doSaveWorldAs() {
        Extern.SceneSaveWorldAs( );
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

    @mainMenuItem( "File/Open Project", false, false )
    static function doOpenProject() {
        Extern.ProjectOpenNew( );
    }
}