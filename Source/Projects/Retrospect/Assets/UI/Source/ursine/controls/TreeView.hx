package ursine.controls;

@:native( "TreeViewControl" )
extern class TreeView extends PolymerElement {
    var empty : Bool;

    function new() : Void;

    function setAsRoot(root : Bool) : Void;
    function findItem(text : String) : TreeViewItem;

    function close() : Void;
}
