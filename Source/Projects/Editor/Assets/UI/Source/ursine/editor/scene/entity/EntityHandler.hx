package ursine.editor.scene.entity;

import ursine.editor.scene.component.ComponentInspection;

@:native( "EntityHandler" )
extern class EntityHandler {
    public function new(uniqueID : UInt) : Void;

    public function isValid() : Bool;

    public function getName() : String;

    public function inspect() : Array<ComponentInspection>;

    public function updateComponentField(componentName : String, fieldName : String, value : Dynamic) : Void;
}