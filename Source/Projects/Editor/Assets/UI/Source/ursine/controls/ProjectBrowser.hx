package ursine.controls;

@:native( "ProjectBrowserControl" )
extern class ProjectBrowser extends PolymerElement {
    function new(resourceTree : Dynamic) : Void;

    function addResource(resourceData : Dynamic) : Void;
}
