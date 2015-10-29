package ursine.controls.inspection;

@:native( "FieldInspectorControl" )
extern class FieldInspector extends PolymerElement {
    var heading : String;

    var container : js.html.DOMElement;

    function new() : Void;
}
