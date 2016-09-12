package ursine.controls;

@:native( "ProjectBrowserControl" )
extern class ProjectBrowser extends PolymerElement {
    function new(resourceTree : Dynamic) : Void;

    function addResource(resourceData : Dynamic) : Void;
    function removeResource(resourceData : Dynamic) : Void;
    function renameResource(resourceData : Dynamic, oldName : String) : Void;
}
