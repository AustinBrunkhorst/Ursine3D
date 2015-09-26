package ursine.controls;

@:native("MenuItemControl")
extern class MenuItem extends PolymerElement {
    var text : String;

    var menu : Menu;

    public function new() : Void;
}
