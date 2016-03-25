package ursine.editor.scene;

@:enum
abstract ScenePlayState(UInt) {
    inline var Playing = 0;
    inline var Paused = 1;
    inline var InEditor = 2;
}
