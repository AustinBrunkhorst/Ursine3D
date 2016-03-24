package ursine.api.ui;

import ursine.api.ui.Screen;

import ursine.api.timers.TimerManager;

@:native( "NativeScreenManager" )
extern class NativeScreenManager {
    function new() : Void;

    function createScreen(path : String, inputBlocking : Bool, priority : Int) : ScreenID;
    function removeScreen(id : ScreenID) : Void;

    function screenHasFocus(id : ScreenID) : Bool;
}

@:keep
@:keepInit
@:keepSub
interface ScreenManager {
    function getScreen(id : ScreenID) : Screen;

    function addScreen(path : String, initData : Dynamic, inputBlocking : Bool = true, priority : Int = 0) : ScreenID;
    function removeScreen(screen : Screen) : Void;

    function hasInputFocus(screen : Screen) : Bool;
}
