package ursine.controls;

@:native( "ColorInputControl" )
extern class ColorInput extends js.html.InputElement {
    function new(color : Dynamic) : Void;

    function show(x : Int, y : Int) : Void;
}
