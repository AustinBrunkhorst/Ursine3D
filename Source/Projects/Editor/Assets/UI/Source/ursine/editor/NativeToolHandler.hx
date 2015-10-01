package ursine.editor;

@:native( "NativeToolHandler" )
extern class NativeToolHandler {
    public function new(toolName : String, ?data : Dynamic) : Void;

    public function Event(name: String, ?data : Dynamic) : Bool;
}
