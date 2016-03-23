package ursine.editor.scene;

@:enum
abstract ScenePlayState(UInt) {
    var Playing = 0;
    var Paused = 1;
    var InEditor = 2;
}
