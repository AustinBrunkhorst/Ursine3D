package ursine.controls.docking;

@:native( "DockContainerControl" )
extern class DockContainer extends PolymerElement {
    var parent(default, null) : js.html.DOMElement;
    var columns(default, null) : Array<DockColumn>;

    function new() : Void;

    function setHeightPercent(height : Float) : Void;

    function addColumn() : DockColumn;
}
