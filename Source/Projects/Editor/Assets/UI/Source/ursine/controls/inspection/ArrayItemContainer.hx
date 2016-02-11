package ursine.controls.inspection;

import ursine.editor.scene.component.inspectors.FieldInspectionHandler;

@:native( "ArrayItemContainerControl" )
extern class ArrayItemContainer extends PolymerElement {
    var index : UInt;
    var opened : Bool;

    var container : js.html.DOMElement;
    var handler : FieldInspectionHandler;

    function new() : Void;
}
