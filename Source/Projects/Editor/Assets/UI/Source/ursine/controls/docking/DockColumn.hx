package ursine.controls.docking;

@:native( "DockColumnControl" )
extern class DockColumn extends PolymerElement {
    var parent : DockContainer;

    function new(parent : DockContainer) : Void;

    function setWidthPercent(width : Float) : Void;

    function addNode(node : DockNode) : Void;
}
