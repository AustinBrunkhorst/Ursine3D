package ursine.editor.menus;

@menuIndex( 4 )
class PhysicsMenu extends MenuItemHandler {
    @mainMenuItem( "Physics/Generate Collider For Model" )
    static function doGenerateColliderForModel() {
        ursine.native.Extern.GenerateColliders( );
    }
}
