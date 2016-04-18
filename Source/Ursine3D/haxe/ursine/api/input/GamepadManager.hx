package ursine.api.input;

import ursine.api.input.GamepadButton;

typedef GamepadIndex = Int;

abstract GamepadEventType(String) {
    public static inline var ButtonDown = "GamepadButtonDown";
    public static inline var ButtonUp = "GamepadButtonUp";
    public static inline var Connected = "GamepadConnected";
    public static inline var Disconnected = "GamepadDisconnected";
}

extern class GamepadButtonEvent {
    var index : GamepadIndex;
    var button : GamepadButtonType;
    var pressed : Bool;
    var triggered : Bool;
}

extern class GamepadConnectionEvent {
    var index : GamepadIndex;
    var connected : Bool;
}

@:native( "NativeGamepadManager" )
extern class GamepadManager {
    static var instance : GamepadManager;

    function new() : Void;

    function numConnected() : Int;
    function isConnected(gamepad : GamepadIndex) : Bool;

    function isButtonDown(gamepad : GamepadIndex, button : GamepadButtonType) : Bool;
    function isButtonUp(gamepad : GamepadIndex, button : GamepadButtonType) : Bool;

    function isButtonTriggeredDown(gamepad : GamepadIndex, button : GamepadButtonType) : Bool;
    function isButtonTriggeredUp(gamepad : GamepadIndex, button : GamepadButtonType) : Bool;
}