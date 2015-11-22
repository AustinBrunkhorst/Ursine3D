package ursine.input;

import ursine.input.KeyboardKey;

class KeyboardEvent {
    public static var KeyDown = "KeyboardKeyDown";
    public static var KeyUp = "KeyboardKeyUp";
}

@:native( "KeyboardManager" )
extern class KeyboardManager {
    function new() : Void;

    function isKeyDown(key : KeyboardKey) : Bool;
    function isKeyUp(key : KeyboardKey) : Bool;

    function isKeyTriggeredDown(key : KeyboardKey) : Bool;
    function isKeyTriggeredUp(key : KeyboardKey) : Bool;
}