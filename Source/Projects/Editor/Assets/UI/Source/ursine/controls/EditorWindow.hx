package ursine.controls;

import ursine.controls.docking.DockNode;
import js.html.DOMElement;

@:native( "EditorWindowControl" )
extern class EditorWindow extends DockNode {
    var heading : String;
    var container : DOMElement;

    function new() : Void;
}
