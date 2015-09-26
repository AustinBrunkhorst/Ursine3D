import ursine.editor.MenuItemContainer;
import ursine.editor.Editor;

import ursine.controls.Menu;
import ursine.controls.MenuItem;

/*class FileMenu extends MenuItemContainer {
    @mainMenuItem( "File/New" )
    static function doNew() {
        trace( "New File!" );
    }

    @mainMenuItem( "File/Open" )
    static function doOpen() {
        trace( "Open File!" );
    }
}*/

class Application extends MenuItemContainer {
    static function main() {
        var editor = new Editor( );

        var file = new MenuItem( );

        file.text = "File";

        var itemNew = new MenuItem( );

        itemNew.text = "New";

        var itemOpen = new MenuItem( );

        itemOpen.text = "Open";

        file.menu.appendChild( itemNew );
        file.menu.appendChild( itemOpen );

        editor.mainMenu.appendChild( file );

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
