package ursine.editor.scene.component.inspectors;

import ursine.editor.scene.component.ComponentDatabase;

interface IFieldInspectionOwner {
    function getFieldHandlers() : Array<FieldInspectionHandler>;

    function notifyChanged(handler : FieldInspectionHandler, field : NativeField, value : Dynamic) : Void;
}
