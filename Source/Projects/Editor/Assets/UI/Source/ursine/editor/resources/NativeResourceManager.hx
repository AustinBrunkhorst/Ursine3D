package ursine.editor.resources;

import ursine.native.Extern;

import ursine.editor.windows.OutputLog;

extern class ResourceBuildError {
    var message : String;
    var file : String;
    var functionName : String;
    var line : UInt;
}

extern class ResourceBuildEventArgs {
    var guid : String;
    var duration : Int;
    var successful : Bool;
    var error : ResourceBuildError;
}

class NativeResourceManager {
    public function new(broadcastManager : NativeBroadcastManager) {
        broadcastManager.getChannel( 'ResourcePipeline' )
            .on( 'ResourceBuildStart', onResourceBuildStart )
            .on( 'ResourceBuildComplete', onResourceBuildComplete );
    }

    private function onResourceBuildStart(e : ResourceBuildEventArgs) {
        var resource : ResourceItem = Extern.ProjectGetResource( e.guid );

        if (resource == null)
            throw 'Failed to get resource from GUID ${e.guid}';

        OutputLog.log( 'Building ${resource.relativePathDisplayName}...' );
    }

    private function onResourceBuildComplete(e : ResourceBuildEventArgs) {
        var resource : ResourceItem = Extern.ProjectGetResource( e.guid );

        if (resource == null)
            throw 'Failed to get resource from GUID ${e.guid}';

        if (e.successful) {
            OutputLog.log( 'Built in ${e.duration} ms: ${resource.relativePathDisplayName}' );
        } else {
            OutputLog.log( 'Build Failed: ${resource.relativePathDisplayName}<br>Reason: ${e.error.message}' );
        }
    }
}
