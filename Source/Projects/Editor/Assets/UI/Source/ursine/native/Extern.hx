package ursine.native;

class Extern {
    public static function DebugEditorUI() {
        return untyped __js__( "DebugEditorUI()" );
    }

    public static function CreateEntity() {
        return untyped __js__( "CreateEntity()" );
    }

    public static function CreateEntityFromArchetype() {
        return untyped __js__( "CreateEntityFromArchetype()" );
    }

    public static function GetNativeComponentDatabase() {
        return untyped __js__( "GetNativeComponentDatabase()" );
    }

    public static function SceneGetRootEntities() {
        return untyped __js__( "SceneGetRootEntities()" );
    }

    public static function SceneGetActiveEntities() {
        return untyped __js__( "SceneGetActiveEntities()" );
    }

    public static function SceneLoad() {
        return untyped __js__( "SceneLoad()" );
    }

    public static function SceneSave() {
        return untyped __js__( "SceneSave()" );
    }

    public static function ScenePlay(playing : Bool) {
        return untyped __js__( "ScenePlay(playing)" );
    }

    public static function SceneStep() {
        return untyped __js__( "SceneStep()" );
    }

    public static function GenerateColliderForModel() {
        return untyped __js__( "GenerateColliderForModel()" );
    }

    public static function ReduceConvexHull() {
        return untyped __js__( "ReduceConvexHull()" );
    }

    public static function GenerateCollidersForScene() {
        return untyped __js__( "GenerateCollidersForScene()" );
    }

    public static function NotificationButtonCallback(id : UInt, buttonID : UInt) {
        return untyped __js__( "notification_NotificationButtonCallback(id, buttonID)" );
    }

    public static function NotificationCloseCallback(id : UInt) {
        return untyped __js__( "notification_NotificationCloseCallback(id)" );
    }

    public static function GenerateBvhTriangleColliderForModel() {
        return untyped __js__( "GenerateBvhTriangleColliderForModel()" );
    }

    public static function GenerateBvhTriangleColliderForScene() {
        return untyped __js__( "GenerateBvhTriangleColliderForScene()" );
    }
}
