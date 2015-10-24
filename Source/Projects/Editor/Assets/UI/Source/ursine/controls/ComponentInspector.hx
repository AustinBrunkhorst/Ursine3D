package ursine.controls;

@:native( "ComponentInspectorControl" )
extern class ComponentInspector extends PolymerElement {
    var heading : String;

    var container : js.html.DOMElement;
    var fieldInspectors : js.html.DOMElement;

    function new() : Void;
}
