package ursine.editor.menus;

@menuIndex( 4 )
class PhysicsMenu extends MenuItemHandler {
    @mainMenuItem( "Physics/Model3D: Generate Collider For Model" )
    static function doGenerateColliderForModel() {
        ursine.native.Extern.GenerateColliderForModel( );
    }

    @mainMenuItem( "Physics/Model3D: Reduce Convex Hull" )
    static function doReduceConvexHull() {
        ursine.native.Extern.ReduceConvexHull( );
    }

    @mainMenuItem( "Physics/FBXSceneRootNode: Generate Colliders For Scene" )
    static function doGenerateCollidersForScene() {
        ursine.native.Extern.GenerateCollidersForScene( );
    }
}
