package ursine.editor.scene.entity;

import ursine.editor.scene.component.ComponentInspection;

@:native( "EntityHandler" )
extern class EntityHandler {
    function new(uniqueID : UInt) : Void;

    function isValid() : Bool;
    function isRemovalEnabled() : Bool;
    function isHierarchyChangeEnabled() : Bool;
    function isVisibleInEditor() : Bool;

    function remove() : Void;

    function getName() : String;
    function setName(name : String) : Void;

    function inspect() : Array<ComponentInspection>;

    function hasComponent(name : String) : Bool;
    function addComponent(name : String) : Void;
    function removeComponent(name : String) : Void;

    function componentFieldUpdate(componentName : String, fieldName : String, value : Dynamic) : Void;

    function componentFieldArrayUpdate(componentName : String, fieldName : String, index : UInt, value : Dynamic) : Void;
    function componentFieldArrayInsert(componentName : String, fieldName : String, index : UInt, value : Dynamic) : Void;
    function componentFieldArrayRemove(componentName : String, fieldName : String, index : UInt) : Void;
    function componentFieldArrayGetLength(componentName : String, fieldName : String) : UInt;

    function componentButtonInvoke(componentName : String, buttonName : String) : Void;

    function getChildren() : Array<UInt>;
    function getParent() : UInt;
    function setParent(parentUniqueID : UInt) : Bool;
    function getSiblingIndex() : UInt;
    function setSiblingIndex(index : UInt) : Void;

    function saveAsArchetype() : Void;
    function clone() : UInt;
}