package ursine.controls;

@:native( "ContextMenuControl" )
extern class ContextMenu extends Menu {
    function new() : Void;

    function open(x : Int, y : Int) : Void;
    function addItem(text : String, callback : Dynamic) : MenuItem;
}
