package ursine.editor.scene.entity;

abstract EntityEvent(String) {
    public static inline var EntityAdded = "EntityAdded";
    public static inline var EntityRemoved = "EntityRemoved";
    public static inline var EntityNameChanged = "EntityNameChanged";
    public static inline var EntityParentChanged = "EntityParentChanged";

    public static inline var ComponentAdded = "ComponentAdded";
    public static inline var ComponentRemoved = "ComponentRemoved";
    public static inline var ComponentChanged = "ComponentChanged";
    public static inline var ComponentArrayInserted = "ComponentArrayInserted";
    public static inline var ComponentArraySet = "ComponentArraySet";
    public static inline var ComponentArrayRemove = "ComponentArrayRemove";
}
