package ursine.editor.menus;

import ursine.editor.scene.entity.Entity;
import ursine.editor.windows.SceneOutline;

@menuIndex( 2 )
class EntityMenu extends MenuItemHandler {
    @mainMenuItem( "Entity/Create/Empty" )
    static function doCreateEmpty() {
        createEntity( "Empty Entity" );
    }

    @mainMenuItem( "Entity/Create/From Archetype" )
    static function doCreateFromArchetype() {
        Entity.createFromArchetype( );
    }

    @mainMenuItem( "Entity/Create/Plane", true )
    static function doCreatePlane() {
        var entity = createEntityWithComponents( "Plane", [ "BoxCollider" ] );

        entity.updateComponentField( "Transform", "scale", {
            x: 5,
            y: 1,
            z: 5
        } );
    }

    @mainMenuItem( "Entity/Create/Box" )
    static function doCreateBox() {
        var entity = createEntityWithComponents( "Box", [ "BoxCollider" ] );
    }

    @mainMenuItem( "Entity/Create/Cylinder" )
    static function doCreateCylinder() {
        var entity = createEntityWithComponents( "Cylinder", [ "CylinderCollider" ] );
    }

    @mainMenuItem( "Entity/Create/Sphere" )
    static function doCreateSphere() {
        var entity = createEntityWithComponents( "Sphere", [ "SphereCollider" ] );
    }

    @mainMenuItem( "Entity/Create/Point Light", true )
    static function doCreatePointLight() {
        var entity = createLight( "Point Light", "Point" );
    }

    @mainMenuItem( "Entity/Create/Spot Light" )
    static function doCreateSpotLight() {
        var entity = createLight( "Spot Light", "Spot" );
    }

    @mainMenuItem( "Entity/Create/Directional Light" )
    static function doCreateDirectionalLight() {
        var entity = createLight( "Directional Light", "Directional" );
    }

    private static function createEntity(name : String) : Entity {
        var entity = Entity.create( );

        entity.setName( name );

        SceneOutline.instance.clearSelectedEntities( );

        entity.select( );

        return entity;
    }

    private static function createEntityWithComponents(name : String, components : Array<String>) : Entity {
        var entity = Entity.create( );

        entity.setName( name );

        for (comp in components) {
            entity.addComponent( comp );
        }

        return entity;
    }

    private static function createLight(name : String, type : String) : Entity {
        var entity = createEntity( name );

        entity.addComponent( "Light" );

        var lightType = Editor.instance.componentDatabase.getNativeType(
            "ursine::ecs::LightType"
        ).enumValue;

        // TODO: create defines for this
        entity.updateComponentField( "Light", "Type", Reflect.field( lightType, type ) );

        return entity;
    }

}
