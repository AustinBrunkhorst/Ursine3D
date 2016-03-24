package ursine.api.input;

import ursine.api.input.KeyboardKey;

abstract KeyboardEventType(String) {
    public static inline var KeyDown = "KeyboardKeyDown";
    public static inline var KeyUp = "KeyboardKeyUp";
}

extern class KeyboardKeyEvent {
    var key : KeyboardKeyType;
    var pressed : Bool;
    var triggered : Bool;
}

@:native( "NativeKeyboardManager" )
extern class KeyboardManager {
    static var instance : KeyboardManager;

    function new() : Void;

    function isKeyDown(key : KeyboardKeyType) : Bool;
    function isKeyUp(key : KeyboardKeyType) : Bool;

    function isKeyTriggeredDown(key : KeyboardKeyType) : Bool;
    function isKeyTriggeredUp(key : KeyboardKeyType) : Bool;
}