package ursine.controls;

@:native( "MenuControl" )
extern class Menu extends PolymerElement {
    var empty : Bool;

    public function new() : Void;

    public function findItem(text : String) : MenuItem;

    public function close() : Void;
}
