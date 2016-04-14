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

        // TODO:
        m_nativeHandler = new NativeToolHandler( toolName );

        hookViewportEvents( window );

        window.addEventListener( 'resize', onViewportInvalidated );
        js.Browser.window.addEventListener( 'resize', onViewportInvalidated );
    }

    public function hookViewportEvents(target : js.html.Element) {
        for (event in m_forwardedEvents)
            target.addEventListener( event, forwardEvent );
    }

    public function unHookViewportEvents(target : js.html.Element) {
        for (event in m_forwardedEvents)
            target.removeEventListener( event, forwardEvent );
    }

    private function forwardEvent(e : Event) {
        m_nativeHandler.Event( e.type, e );
    }

    // TODO: make private after dock calls made
    public function onViewportInvalidated() {
        var bounds = window.container.getBoundingClientRect( );

        m_nativeHandler.Event( 'viewportInvalidated', {
            x: bounds.left * js.Browser.window.devicePixelRatio,
            y: bounds.top * js.Browser.window.devicePixelRatio,
            width: bounds.width * js.Browser.window.devicePixelRatio,
            // @@@TODO: to account for resize border
            height: (bounds.height - 5) * js.Browser.window.devicePixelRatio
        } );
    }
}
