package ursine.input;

@:enum
abstract GamepadButton(Int) {
    var INVALID           = -1;
    var A                 = 0;
    var B                 = 1;
    var X                 = 2;
    var Y                 = 3;
    var DPAD_LEFT         = 13;
    var DPAD_RIGHT        = 14;
    var DPAD_UP           = 11;
    var DPAD_DOWN         = 12;
    var STICK_LEFT        = 7;
    var STICK_RIGHT       = 8;
    var SHOULDER_LEFT     = 9;
    var SHOULDER_RIGHT    = 10;
    var BACK              = 4;
    var START             = 6;
    var GUIDE             = 5;
    var MAX               = 15;
    var TRIGGER_LEFT      = 16;
    var TRIGGER_RIGHT     = 17;
    var STICK_LEFT_UP     = 18;
    var STICK_LEFT_DOWN   = 19;
    var STICK_LEFT_LEFT   = 20;
    var STICK_LEFT_RIGHT  = 21;
    var STICK_RIGHT_UP    = 22;
    var STICK_RIGHT_DOWN  = 23;
    var STICK_RIGHT_LEFT  = 24;
    var STICK_RIGHT_RIGHT = 25;
    var END               = 26;
}