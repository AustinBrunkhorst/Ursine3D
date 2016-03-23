package ursine.api.input;

import ursine.api.input.KeyboardKey;

abstract KeyboardEventType(String) {
    public static inline var KeyDown = "KeyboardKeyDown";
    public static inline var KeyUp = "KeyboardKeyUp";
}

extern class KeyboardKeyEvent {
    var key : KeyboardKey;
    var pressed : Bool;
    var triggered : Bool;
}

@:native( "NativeKeyboardManager" )
extern class KeyboardManager {
    static var instance : KeyboardManager;

    function new() : Void;

    function isKeyDown(key : KeyboardKey) : Bool;
    function isKeyUp(key : KeyboardKey) : Bool;

    function isKeyTriggeredDown(key : KeyboardKey) : Bool;
    function isKeyTriggeredUp(key : KeyboardKey) : Bool;
}