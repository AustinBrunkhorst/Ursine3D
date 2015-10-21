package ursine.controls;

@:native( "FieldInspectorControl" )
extern class FieldInspector extends PolymerElement {
    public var heading : String;

    var container : js.html.DOMElement;

    public function new() : Void;
}
