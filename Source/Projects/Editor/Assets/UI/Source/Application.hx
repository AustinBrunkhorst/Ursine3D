import ursine.controls.MenuSeparator;
import ursine.editor.MenuItemContainer;
import ursine.editor.Editor;

import ursine.controls.MainMenu;
import ursine.controls.MenuItem;

class FileMenu extends MenuItemContainer {
    @mainMenuItem( "File/New" )
    static function doNew() {
        trace( "New File!" );
    }

    @mainMenuItem( "File/Open" )
    static function doOpen() {
        trace( "Open File!" );
    }
}

class MenuWrapper {
    public var text : String;

    public var children : Array<MenuWrapper>;

    public function new(text : String, children : Array<MenuWrapper>) {
        this.text = text;
        this.children = children;
    }
}

class Application extends MenuItemContainer {
    static function buildMenus(menus : Array<MenuWrapper>) {
        for (item in menus)
            buildMenu( item, Editor.instance.mainMenu );
    }

    static function buildMenu(menu : MenuWrapper, parent : ursine.controls.Menu) {
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
            new MenuWrapper( "Two", [ new MenuWrapper( "One One", [ ] ),
            new MenuWrapper( "One Two", [ ] ),
            new MenuWrapper( "One Three",
            [
                new MenuWrapper( "One Three One", [ ] ),
                new MenuWrapper( "One Three Two", [ ] ),
                new MenuWrapper( "One Three Three", [ new MenuWrapper( "One One", [ ] ),
                new MenuWrapper( "One Two", [ ] ),
                new MenuWrapper( "One Three",
                [
                    new MenuWrapper( "One Three One", [ ] ),
                    new MenuWrapper( "One Three Two", [ ] ),
                    new MenuWrapper( "One Three Three", [ new MenuWrapper( "One One", [ ] ),
                    new MenuWrapper( "One Two", [ ] ),
                    new MenuWrapper( "One Three",
                    [
                        new MenuWrapper( "One Three One", [ ] ),
                        new MenuWrapper( "One Three Two", [ ] ),
                        new MenuWrapper( "One Three Three", [ new MenuWrapper( "One One", [ ] ),
                        new MenuWrapper( "One Two", [ ] ),
                        new MenuWrapper( "One Three",
                        [
                            new MenuWrapper( "One Three One", [ ] ),
                            new MenuWrapper( "One Three Two", [ ] ),
                            new MenuWrapper( "One Three Three", [ ] ),
                            new MenuWrapper( "One Three Four", [ ] )
                        ]
                        ),
                        new MenuWrapper( "One Four", [ ] ) ] ),
                        new MenuWrapper( "One Three Four", [ ] )
                    ]
                    ),
                    new MenuWrapper( "One Four", [ ] ) ] ),
                    new MenuWrapper( "One Three Four", [ ] )
                ]
                ),
                new MenuWrapper( "One Four", [ ] ) ] ),
                new MenuWrapper( "One Three Four", [ ] )
            ]
            ),
            new MenuWrapper( "One Four", [ ] ) ] ),
            new MenuWrapper( "Three", [ ] )
        ];

        buildMenus( menus );

        var classTypeNames : Array<String> = untyped __js__( "Object.keys( $hxClasses )" );

        for (className in classTypeNames) {
            var classType = Type.resolveClass( className );

            if (!haxe.rtti.Rtti.hasRtti( classType ))
                continue;

            var rtti = haxe.rtti.Rtti.getRtti( classType );

            for (field in rtti.statics) {
                // ignore non-functions
                if (Type.enumConstructor( field.type ) != "CFunction")
                    continue;

                // attempt to find the mainMenuItem annotation
                var menuItem = Lambda.find( field.meta, function(meta) {
                    return meta.name == "mainMenuItem";
                } );

                if (menuItem == null)
                    continue;

                // retrieve the method handle
                var method = Reflect.field( classType, field.name );

                // TODO: expand into sub menus
                var title = StringTools.replace( menuItem.params[ 0 ], '"', "" ).split( "/" ).pop( );

                var item = new MenuItem( );

                item.text = title;

                item.addEventListener( 'click', function() {
                    Reflect.callMethod( classType, method, [ ] );
                } );

                Editor.instance.mainMenu.appendChild( item );
            }
        }
    }
}
