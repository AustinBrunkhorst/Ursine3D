package ursine.controls;

@:native( "TreeViewItemControl" )
extern class TreeViewItem extends PolymerElement {
    var text : String;

    var child : TreeView;

    function new() : Void;
}
