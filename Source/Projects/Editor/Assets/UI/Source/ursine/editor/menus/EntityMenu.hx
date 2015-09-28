package ursine.editor.menus;

@menuIndex( 2 )
class EntityMenu extends MenuItemHandler {
    @mainMenuItem( "Entity/Create/Empty" )
    static function doCreateEmpty() {
        trace( "create empty" );
    }

    @mainMenuItem( "Entity/Create/Plane", true )
    static function doCreatePlane() {
        trace( "create plane" );
    }

    @mainMenuItem( "Entity/Create/Box" )
    static function doCreateBox() {
        trace( "create box" );
    }

    @mainMenuItem( "Entity/Create/Cylinder" )
    static function doCreateCylinder() {
        trace( "create Cylinder" );
    }

    @mainMenuItem( "Entity/Create/Sphere" )
    static function doCreateSphere() {
        trace( "create Sphere" );
    }

    @mainMenuItem( "Entity/Create/Point Light", true )
    static function doCreatePointLight() {
        trace( "create Point Light" );
    }

    @mainMenuItem( "Entity/Create/Spot Light" )
    static function doCreateSpotLight() {
        trace( "create Spot Light" );
    }

    @mainMenuItem( "Entity/Create/Directional Light" )
    static function doCreateDirectionalLight() {
        trace( "create Directional Light" );
    }
}
