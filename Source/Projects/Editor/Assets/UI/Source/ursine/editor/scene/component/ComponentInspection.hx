package ursine.editor.scene.component;

extern class ComponentEditorButton {
    var name : String;
    var text : String;
}

extern class ComponentInspection {
    var type : String;
    var value : Dynamic;
    var buttons : Array<ComponentEditorButton>;
}