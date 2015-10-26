package ursine.editor.scene.entity;

import ursine.editor.scene.component.ComponentInspection;

@:native( "EntityHandler" )
extern class EntityHandler {
    function new(uniqueID : UInt) : Void;

    function isValid() : Bool;

    function getName() : String;
    function setName(name : String) : Void;

    function inspect() : Array<ComponentInspection>;

    function hasComponent() : Bool;
    function addComponent(name : String) : Void;
    function removeComponent(name : String) : Void;

    function updateComponentField(componentName : String, fieldName : String, value : Dynamic) : Void;
}