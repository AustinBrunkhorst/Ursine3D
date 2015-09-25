import ursine.Editor;

import ursine.utils.EventManager;

import ursine.menu.MenuItem;

class Application {
    static function main() {
        var editor = new Editor( );

        var item = new MenuItem( 'test' );

        item.events.one( 'click', function(e) {
            trace( 'test' );
        } );

        editor.mainMenu.addItem( item );
    }
}
