package ursine.controls;

@:native( "MenuItemControl" )
extern class MenuItem extends PolymerElement {
    var disabled : Bool;
    var text : String;
    var icon : String;

    var menu : Menu;

    function new() : Void;
}
