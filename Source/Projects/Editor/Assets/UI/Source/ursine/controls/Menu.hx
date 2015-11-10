package ursine.controls;

@:native( "MenuControl" )
extern class Menu extends PolymerElement {
    var empty : Bool;

    function new() : Void;

    function findItem(text : String) : MenuItem;

    function close() : Void;
}
