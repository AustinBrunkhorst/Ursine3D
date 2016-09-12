package ursine.api.ui;

import ursine.api.ui.Screen;
import ursine.api.events.EventManager;
import ursine.api.timers.TimerManager;

@:native( "NativeScreenManager" )
extern class NativeScreenManager {
    function new() : Void;

    function createScreen(path : String, inputBlocking : Bool, priority : Int) : ScreenID;
    function removeScreen(id : ScreenID) : Void;

    function screenHasFocus(id : ScreenID) : Bool;

    function messageOwner(screen : ScreenID, message : String, data : Dynamic) : Void;
    function messageGlobal(message : String, data : Dynamic) : Void;
}

@:keep
@:keepInit
@:keepSub
interface ScreenManager {
    var globalEvents : EventManager;

    function getScreen(id : ScreenID) : Screen;

    function addScreen(path : String, initData : Dynamic, inputBlocking : Bool = true, priority : Int = 0) : ScreenID;
    function removeScreen(screen : Screen) : Void;

    function hasInputFocus(screen : Screen) : Bool;

    function messageNativeOwner(screen : Screen, message : String, data : Dynamic) : Void;
    function messageNativeGlobal(message : String, data : Dynamic) : Void;
}
