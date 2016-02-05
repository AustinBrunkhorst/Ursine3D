package ursine.native;

class Extern {
    public static function DebugEditorUI() {
        return untyped __js__( "DebugEditorUI()" );
    }

    public static function ProcessOpen(path, relative: Bool = false) {
        return untyped __js__( "ProcessOpen(path, relative)" );
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

    public static function ScenePlayStart() {
        return untyped __js__( "ScenePlayStart()" );
    }

    public static function SceneSetPlayState(playing : Bool) {
        return untyped __js__( "SceneSetPlayState(playing)" );
    }

    public static function SceneStep() {
        return untyped __js__( "SceneStep()" );
    }

    public static function ScenePlayStop() {
        return untyped __js__( "ScenePlayStop()" );
    }

    public static function SceneGetEntitySystems() {
        return untyped __js__( "SceneGetEntitySystems()" );
    }

    public static function NotificationButtonCallback(id : UInt, buttonID : UInt) {
        return untyped __js__( "ursine_NotificationButtonCallback(id, buttonID)" );
    }

    public static function NotificationCloseCallback(id : UInt) {
        return untyped __js__( "ursine_NotificationCloseCallback(id)" );
    }

    public static function WaypointUnidirectionalConnector() {
        return untyped __js__( "WaypointUnidirectionalConnector()" );
    }

    public static function WaypointBidirectionalConnector() {
        return untyped __js__( "WaypointBidirectionalConnector()" );
    }

    public static function WaypointEnableDrawLines() {
        return untyped __js__( "WaypointEnableDrawLines()");
    }

    public static function WaypointDisableDrawLines() {
        return untyped __js__( "WaypointDisableDrawLines()");
    }
}
