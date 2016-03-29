package ursine.editor.resources;

extern class ResourceItem {
    var guid : String;
    var type : String;
    var displayName : String;
    var relativePathDisplayName : String;
    var sourceFile : String;
    var isGenerated : Bool;
    var hasPreview : Bool;
    var previewFile : String;
    var buildFile : String;
    var buildCacheFile : String;
    var extension : String;
}