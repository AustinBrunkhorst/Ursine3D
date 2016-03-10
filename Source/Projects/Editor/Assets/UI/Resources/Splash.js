var gProgressSpan = null;
var gProgressText = null;

var gBroadcastHandlers = {
    ResourcePipeline: {
        preBuildItemStart: function(data) {
            gProgressSpan.style.width = (data.progress * 100) + '%';
            gProgressText.innerHTML = 'Building '+ data.item;
        },
        preBuildItemPreviewStart: function(data) {
            gProgressSpan.style.width = (data.progress * 100) + '%';
            gProgressText.innerHTML = 'Preview '+ data.item;
        },
        preBuildComplete: function(data) {
            gProgressSpan.style.width = '100%';
            gProgressText.innerHTML = 'Launching Editor';
        }
    }
};

window.addEventListener( 'load', function() {
    document.querySelector( '#project-name' ).innerHTML = ProjectGetName( );

    gProgressSpan = document.querySelector( '#progress span' );
    gProgressText = document.querySelector( '#progress-text' );
} );

function NativeBroadcast(target, message, data) {
    var targetHandler = gBroadcastHandlers[ target ];

    if (targetHandler) {
        var messageHandler = targetHandler[ message ];

        if (messageHandler)
            messageHandler( data );
    }
}