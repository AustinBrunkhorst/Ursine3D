package ursine.controls.docking;

@:native( "DockColumnControl" )
extern class DockColumn extends PolymerElement {
    var parent(default, null) : DockContainer;
    var container(default, null) : DockContainer;
    var rows(default, null) : Array<DockContainer>;

    function new(parent : DockContainer) : Void;

    function setWidthPercent(width : Float) : Void;

    function addRow() : DockContainer;
}
