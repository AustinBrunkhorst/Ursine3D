package ursine.controls;

@:native( "TreeViewItemControl" )
extern class TreeViewItem extends PolymerElement {
    var text : String;
    var selected : Bool;
    var opened : Bool;

    var child : TreeView;

    function new() : Void;
}
