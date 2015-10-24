package ursine.controls.docking;

@:native( "DockContainerControl" )
extern class DockContainer extends PolymerElement {
    var parent : js.html.DOMElement;

    function new() : Void;

    function addColumn() : DockColumn;
}
