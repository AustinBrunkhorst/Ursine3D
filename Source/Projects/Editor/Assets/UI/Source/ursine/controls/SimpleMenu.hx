package ursine.controls;

@:native("Object")
extern class TestItem {
    public var name : String;    public var ordered : Int;

    public function new() : Void;
}

@:native("SimpleMenu")
extern class SimpleMenu extends PolymerElement {
    var menuItems : Array<TestItem>;

    public function new() : Void;
}
