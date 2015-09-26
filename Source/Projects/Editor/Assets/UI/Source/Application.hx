import ursine.Editor;

import ursine.controls.Menu;
import ursine.controls.MenuItem;

class MenuWrapper {
    public var text : String;

    public var children : Array<MenuWrapper>;

    public function new(text : String, children : Array<MenuWrapper>) {
        this.text = text;
        this.children = children;
    }
}

class Application {
    static function buildMenus(menus : Array<MenuWrapper>) {
        var menu = new Menu( );

        menu.classList.add( 'main-menu' );

        for (item in menus)
            buildMenu( item, menu );

        js.Browser.document.querySelector( '#header-toolbar' ).appendChild( menu );
    }

    static function buildMenu(menu : MenuWrapper, parent : Menu) {
        var item = new MenuItem( );

        item.text = menu.text;

        parent.appendChild( item );

        for (child in menu.children) {
            buildMenu( child, item.menu );
        }
    }

    static function main() {
        var editor = new Editor( );

        var menus = [
            new MenuWrapper( "One",
                [
                    new MenuWrapper( "One One", [ ] ),
                    new MenuWrapper( "One Two", [ ] ),
                    new MenuWrapper( "One Three",
                        [
                            new MenuWrapper( "One Three One", [ ] ),
                            new MenuWrapper( "One Three Two", [ ] ),
                            new MenuWrapper( "One Three Three", [ ] ),
                            new MenuWrapper( "One Three Four", [ ] )
                        ]
                    ),
                    new MenuWrapper( "One Four", [ ] )
                ]
            ),
            new MenuWrapper( "Two", [ ] ),
            new MenuWrapper( "Three", [ ] )
        ];

        buildMenus( menus );
    }
}
