package ursine.editor;

import js.html.Event;

class NativeCanvasWindowHandler extends WindowHandler {
    private var m_nativeHandler : NativeToolHandler;

    // events to be forwareded to the native handler
    private static var m_forwardedEvents : Array<String> = [
        "focus",
        "blur",
        "mouseover",
        "mouseout"
    ];

    public function new(toolName : String) {
        super( );

        m_nativeHandler = new NativeToolHandler( toolName );

        for (event in m_forwardedEvents)
            window.addEventListener( event, forwardEvent );

        js.Browser.window.addEventListener( 'resize', onViewportInvalidated );
    }

    private function resizeOuterWindow(width : Int, height : Int)
    {
        window.style.width = width + 'px';
        window.style.height = height + 'px';

        onViewportInvalidated( );
    }

    private function forwardEvent(e : Event) {
        m_nativeHandler.Event( e.type, e );
    }

    private function onViewportInvalidated() {
        var bounds = window.container.getBoundingClientRect( );

        m_nativeHandler.Event( 'viewportInvalidated', {
            x: bounds.left,
            y: bounds.top,
            width: bounds.width,
            height: bounds.height
        } );
    }
}
