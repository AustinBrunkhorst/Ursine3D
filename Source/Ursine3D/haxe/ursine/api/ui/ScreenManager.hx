package ursine.api.ui;

import ursine.api.ui.Screen;

@:native( "NativeScreenManager" )
extern class NativeScreenManager {
    function new() : Void;

    function createScreen(path : String, initData : Dynamic, inputBlocking : Bool, priority : Int) : ScreenID;
    function removeScreen(id : ScreenID) : Void;

    function screenHasFocus(id : ScreenID) : Bool;
}

@:native( "ScreenManager" )
extern class ScreenManager {
    static var instance : ScreenManager;

    function new() : Void;

    function createScreen(path : String, initData : Dynamic, inputBlocking : Bool = true, priority : Int = 0) : Screen;
}
