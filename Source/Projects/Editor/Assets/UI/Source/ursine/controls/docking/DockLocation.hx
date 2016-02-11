package ursine.controls.docking;

@:enum
abstract DockLocation(Int) {
    var Left = 0;
    var Right = 1;
    var Top = 2;
    var Bottom = 3;
    var Fill = 4;
}