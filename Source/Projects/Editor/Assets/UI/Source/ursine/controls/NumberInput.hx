package ursine.controls;

@:native( "NumberInputControl" )
extern class NumberInput extends js.html.InputElement {
    // determines if this number is treated as a slider
    var slider : Bool;

    // format to display on a slider
    var format : String;

    function new() : Void;
}
