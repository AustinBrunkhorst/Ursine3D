package ursine.controls;

@:native( "ItemSelectionPopupControl" )
extern class ItemSelectionPopup extends PolymerElement {
    function new(items : Array<String>) : Void;

    function show(x : Int, y : Int) : Void;
}