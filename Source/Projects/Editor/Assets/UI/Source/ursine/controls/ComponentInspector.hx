package ursine.controls;

@:native( "ComponentInspectorControl" )
extern class ComponentInspector extends PolymerElement {
    public var heading : String;

    public var container : js.html.DOMElement;
    public var fieldInspectors : js.html.DOMElement;

    public function new() : Void;
}
