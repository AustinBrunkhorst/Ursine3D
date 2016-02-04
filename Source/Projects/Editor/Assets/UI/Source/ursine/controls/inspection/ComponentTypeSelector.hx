package ursine.controls.inspection;

@:native( "ComponentTypeSelectorControl" )
extern class ComponentTypeSelector extends PolymerElement {
    function new(types : Array<String>) : Void;

    function show(x : Int, y : Int) : Void;
}
