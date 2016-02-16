var gFileCallbackProjectLocation = 0;

window.addEventListener( 'load', function() {
    var newProjectModal = document.querySelector( '#new-project-modal' );


    document.querySelector( '#version' ).innerHTML = 'Version '+ GetEditorVersion( );

    document.querySelector( '#actions .new' ).addEventListener( 'click', function() {
        newProjectModal.classList.add( 'active' );
    } );

    document.querySelector( '#btn-project-new-location-browse' ).addEventListener( 'click', function() {
        BrowseFolder( 'Select Project Folder', [ ], gFileCallbackProjectLocation );
    } );

    document.querySelector( '#btn-project-new-create' ).addEventListener( 'click', function() {
        var projectName = document.querySelector( '#project-new-name').value;
        var projectDirectory = document.querySelector( '#project-new-location' ).value;

        LauncherProjectCreate( projectName, projectDirectory );
    } );

    document.querySelector( '#btn-project-new-cancel' ).addEventListener( 'click', function() {
        newProjectModal.classList.remove( 'active' );
    } );
} );

var gFileCallbacks = {
    // new project folder
    0: function(data) {
        document.querySelector( '#project-new-location' ).value = data.file;
    }
};

function NativeBroadcast(target, message, data) {
    if (target == 'FileBrowser' && message == 'fileSelected') {
        gFileCallbacks[ data.callbackID ]( data );
    }
}