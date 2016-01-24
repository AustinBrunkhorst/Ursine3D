package ursine.controls.inspection;

@:native( "ComponentInspectorControl" )
extern class ComponentInspector extends PolymerElement {
    var heading : String;
    var canRemove : Bool;
    var opened : Bool;

    var container : js.html.DOMElement;
    var buttons : js.html.DOMElement;
    var fieldInspectors : js.html.DOMElement;

    function new() : Void;
}
