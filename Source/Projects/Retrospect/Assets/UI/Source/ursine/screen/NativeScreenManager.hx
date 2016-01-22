package ursine.screen;

import ursine.screen.Screen;

@:native( "ScreenManager" )
extern class NativeScreenManager {
    function new() : Void;

    function setScreen(name : String, data : Dynamic) : Void;
    function addOverlay(name : String, data : Dynamic) : Void;
    function removeScreen(id : ScreenID) : Void;
    function removeCurrent() : Void;
    function messageScreen(id : ScreenID, message : String, data : Dynamic) : Void;
    function getFocusedScreen() : ScreenID;
}