package ursine.controls;

@:native( "ItemSelectionPopupControl" )
extern class ItemSelectionPopup extends PolymerElement {
    var currentFilter : String;
    var filterInput : js.html.InputElement;

    function new(items : Array<String>) : Void;

    function show(x : Float, y : Float) : Void;
    function close() : Void;
}