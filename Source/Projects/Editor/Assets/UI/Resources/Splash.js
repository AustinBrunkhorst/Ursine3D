var gProgressSpan = null;
var gProgressText = null;

var gBroadcastHandlers = {
    ResourcePipeline: {
        preBuildItemStart: function(data) {
            setStatus( 'Building '+ data.item, data.progress );
        },
        preBuildItemPreviewStart: function(data) {
            setStatus( 'Preview '+ data.item, data.progress );
        },
        preBuildComplete: function(data) {
            setStatus( 'Launching Editor', 1.0 );
        }
    }
};

window.addEventListener( 'load', function() {
    document.querySelector( '#project-name' ).innerHTML = ProjectGetName( );

    gProgressSpan = document.querySelector( '#progress span' );
    gProgressText = document.querySelector( '#progress-text' );
} );

function setStatus(text, progress) {
    gProgressSpan.style.width = (progress * 100) + '%';
    gProgressText.innerHTML = text;
}

function NativeBroadcast(target, message, data) {
    var targetHandler = gBroadcastHandlers[ target ];

    if (targetHandler) {
        var messageHandler = targetHandler[ message ];

        if (messageHandler)
            messageHandler( data );
    }
}