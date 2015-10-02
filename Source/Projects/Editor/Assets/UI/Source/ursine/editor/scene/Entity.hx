package ursine.editor.scene;

@:native( "EntityHandler" )
extern class Entity {
    public function new(uniqueID : UInt) : Void;

    public function isValid() : Bool;

    public function getName() : String;

    public function inspect() : Array<ComponentInspection>;

    public function updateComponentField(componentName : String, fieldName : String, value : Dynamic) : Void;
}

extern class ComponentInspection {
    var type : String;
    var value : Dynamic;
}
