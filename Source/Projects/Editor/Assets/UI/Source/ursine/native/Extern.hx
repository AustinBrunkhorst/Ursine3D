package ursine.native;

import ursine.editor.scene.ScenePlayState;

class Extern {
    public static function ProcessOpen(path, relative: Bool = false) {
        return untyped __js__( "ProcessOpen(path, relative)" );
    }

    public static inline function GetEditorCommands() {
        return untyped __js__( "editor_commands_GetEditorCommands()" );
    }

    public static inline function CreateEntity() {
        return untyped __js__( "CreateEntity()" );
    }

    public static inline function GetNativeComponentDatabase() {
        return untyped __js__( "GetNativeComponentDatabase()" );
    }

    public static inline function ProjectGetResourceTree() {
        return untyped __js__( "ProjectGetResourceTree()" );
    }

    public static function ProjectGetResourcesByType(type : String) {
        return untyped __js__( "ProjectGetResourcesByType(type)" );
    }

    public static function ProjectGetResource(guid : String) {
        return untyped __js__( "ProjectGetResource(guid)" );
    }

    public static function ProjectDeleteResource(guid : String) {
        return untyped __js__( "ProjectDeleteResource(guid)" );
    }

    public static inline function ProjectSetEmptyScene() {
        return untyped __js__( "ProjectSetEmptyScene()" );
    }

    public static inline function ProjectOpenNew() {
        return untyped __js__( "ProjectOpenNew()" );
    }

    public static inline function SceneSaveWorld() {
        return untyped __js__( "SceneSaveWorld()" );
    }

    public static inline function SceneSaveWorldAs() {
        return untyped __js__( "SceneSaveWorldAs()" );
    }

    public static function SceneSetActiveWorld(guid : String) {
        return untyped __js__( "SceneSetActiveWorld(guid)" );
    }

    public static function SceneInstantiateArchetype(guid : String) {
        return untyped __js__( "SceneInstantiateArchetype(guid)" );
    }

    public static inline function SceneGetRootEntities() {
        return untyped __js__( "SceneGetRootEntities()" );
    }

    public static inline function SceneGetActiveEntities() {
        return untyped __js__( "SceneGetActiveEntities()" );
    }

    public static inline function SceneClearSelectedEntities() {
        return untyped __js__( "SceneClearSelectedEntities()" );
    }

    public static inline function SceneDeleteSelectedEntities() {
        return untyped __js__( "SceneDeleteSelectedEntities()" );
    }

    public static inline function SceneLoad() {
        return untyped __js__( "SceneLoad()" );
    }

    public static inline function SceneSave() {
        return untyped __js__( "SceneSave()" );
    }

    public static inline function SceneGetPlayState() : ScenePlayState {
        return untyped __js__( "SceneGetPlayState()" );
    }

    public static function SceneSetPlayState(state : ScenePlayState) {
        return untyped __js__( "SceneSetPlayState(state)" );
    }

    public static inline function SceneStep() {
        return untyped __js__( "SceneStep()" );
    }

    public static inline function SceneGetEntitySystems() {
        return untyped __js__( "SceneGetEntitySystems()" );
    }

    public static function NotificationButtonCallback(id : UInt, buttonID : UInt) {
        return untyped __js__( "ursine_NotificationButtonCallback(id, buttonID)" );
    }

    public static function NotificationCloseCallback(id : UInt) {
        return untyped __js__( "ursine_NotificationCloseCallback(id)" );
    }

    public static inline function WaypointUnidirectionalConnector() {
        return untyped __js__( "WaypointUnidirectionalConnector()" );
    }

    public static inline function WaypointBidirectionalConnector() {
        return untyped __js__( "WaypointBidirectionalConnector()" );
    }

    public static inline function WaypointEnableDrawLines() {
        return untyped __js__( "WaypointEnableDrawLines()");
    }

    public static inline function WaypointDisableDrawLines() {
        return untyped __js__( "WaypointDisableDrawLines()");
    }
}
