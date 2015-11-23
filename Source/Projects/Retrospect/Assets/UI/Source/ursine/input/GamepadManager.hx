package ursine.input;

import ursine.input.GamepadButton;

typedef GamepadIndex = Int;

class GamepadEventType {
    public static var ButtonDown = "GamepadButtonDown";
    public static var ButtonUp = "GamepadButtonUp";
    public static var Connected = "GamepadConnected";
    public static var Disconnected = "GamepadDisconnected";
}

extern class GamepadButtonEvent {
    var index : GamepadIndex;
    var button : GamepadButton;
    var pressed : Bool;
    var triggered : Bool;
}

extern class GamepadConnectionEvent {
    var index : GamepadIndex;
    var connected : Bool;
}

@:native( "GamepadManager" )
extern class GamepadManager {
    function new() : Void;

    function numConnected() : Int;
    function isConnected(gamepad : GamepadIndex) : Bool;

    function isButtonDown(gamepad : GamepadIndex, button : GamepadButton) : Bool;
    function isButtonUp(gamepad : GamepadIndex, button : GamepadButton) : Bool;

    function isButtonTriggeredDown(gamepad : GamepadIndex, button : GamepadButton) : Bool;
    function isButtonTriggeredUp(gamepad : GamepadIndex, button : GamepadButton) : Bool;
}