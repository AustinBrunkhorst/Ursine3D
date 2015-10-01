package ursine.editor.menus;

@menuIndex( 1 )
class EditMenu extends MenuItemHandler {
    @mainMenuItem( "Edit/Undo" )
    static function doUndo() {
        trace( "undo" );
    }

    @mainMenuItem( "Edit/Redo" )
    static function doRedo() {
        trace( "redo" );
    }
}
