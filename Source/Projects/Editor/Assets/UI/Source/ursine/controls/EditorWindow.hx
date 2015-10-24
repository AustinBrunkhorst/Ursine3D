package ursine.controls;

import js.html.DOMElement;

@:native( "EditorWindowControl" )
extern class EditorWindow extends PolymerElement {
    var heading : String;
    var container : DOMElement;

    public function new() : Void;
}
