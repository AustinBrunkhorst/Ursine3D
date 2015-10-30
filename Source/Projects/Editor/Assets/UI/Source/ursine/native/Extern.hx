package ursine.native;

class Extern {
    public static function DebugEditorUI() {
        return untyped __js__( "DebugEditorUI()" );
    }

    public static function CreateEntity() {
        return untyped __js__( "CreateEntity()" );
    }

    public static function GetNativeComponentDatabase() {
        return untyped __js__( "GetNativeComponentDatabase()" );
    }

    public static function SceneGetActiveEntities() {
        return untyped __js__( "SceneGetActiveEntities()" );
    }
}
