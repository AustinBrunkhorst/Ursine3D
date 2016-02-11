package ursine.editor.menus;

@menuIndex( 2 )
class EntityMenu extends MenuItemHandler {
    @mainMenuItem( "Entity/Create/Empty" )
    static function doCreateEmpty() {
        untyped __js__( "editor_commands_CreateEntity()" );
    }

    @mainMenuItem( "Entity/Create/From Archetype" )
    static function doCreateFromArchetype() {
        untyped __js__( "editor_commands_CreateEntityFromArchetype()" );
    }

    @mainMenuItem( "Entity/Create/Plane", true )
    static function doCreatePlane() {
        untyped __js__( "editor_commands_CreatePlane()" );
    }

    @mainMenuItem( "Entity/Create/Box" )
    static function doCreateBox() {
        untyped __js__( "editor_commands_CreateBox()" );
    }

    @mainMenuItem( "Entity/Create/Cylinder" )
    static function doCreateCylinder() {
        untyped __js__( "editor_commands_CreateCylinder()" );
    }

    @mainMenuItem( "Entity/Create/Sphere" )
    static function doCreateSphere() {
        untyped __js__( "editor_commands_CreateSphere()" );
    }

    @mainMenuItem( "Entity/Create/Point Light", true )
    static function doCreatePointLight() {
        untyped __js__( "editor_commands_CreatePointLight()" );
    }

    @mainMenuItem( "Entity/Create/Spot Light" )
    static function doCreateSpotLight() {
        untyped __js__( "editor_commands_CreateSpotLight()" );
    }

    @mainMenuItem( "Entity/Create/Directional Light" )
    static function doCreateDirectionalLight() {
        untyped __js__( "editor_commands_CreateDirectionalLight()" );
    }
}
