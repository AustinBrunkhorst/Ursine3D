package ursine.controls;

@:native( "ProgressBarControl" )
extern class ProgressBar extends PolymerElement {
    var displayText : String;

    var min : Float;
    var max : Float;
    var value : Float;

    function new() : Void;
}
