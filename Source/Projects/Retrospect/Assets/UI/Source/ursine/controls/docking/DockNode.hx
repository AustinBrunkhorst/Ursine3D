package ursine.controls.docking;

@:native( "DockNodeControl" )
extern class DockNode extends PolymerElement {
   var parent : DockColumn;

   function new() : Void;

   function dock(node : DockNode, location : DockLocation) : DockColumn;
}
