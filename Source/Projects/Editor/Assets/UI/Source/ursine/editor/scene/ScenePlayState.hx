package ursine.editor.scene;

@:enum
abstract ScenePlayState(UInt) {
    inline var InEditor = 0;
    inline var Playing = 1;
    inline var Paused = 2;
}
