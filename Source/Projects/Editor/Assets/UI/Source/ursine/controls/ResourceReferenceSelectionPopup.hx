package ursine.controls;

@:native( "ResourceReferenceSelectionPopupControl" )
extern class ResourceReferenceSelectionPopup extends PolymerElement {
    var currentFilter : String;
    var filterInput : js.html.InputElement;

    function new(resources : Array<Dynamic>) : Void;

    function show(x : Float, y : Float) : Void;
    function close() : Void;
}