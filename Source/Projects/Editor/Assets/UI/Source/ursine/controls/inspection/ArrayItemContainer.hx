package ursine.controls.inspection;

@:native( "ArrayItemContainerControl" )
extern class ArrayItemContainer extends PolymerElement {
    var index : UInt;

    var container : js.html.DOMElement;

    function new() : Void;
}
