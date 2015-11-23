package ursine.screen;

import ursine.screen.Screen;
import ursine.screen.NativeScreenManager;

import ursine.input.GamepadManager;
import ursine.input.KeyboardManager;

class ScreenManager {
    public static var instance : ScreenManager;

    private var m_screenPackagePrefix : String;
    private var m_nativeManager : NativeScreenManager;
    private var m_screens : Map<ScreenID, Screen>;

    private var m_screensContainer : js.html.Element;

    public function new(screenPackagePrefix : String) {
        instance = this;

        m_screenPackagePrefix = screenPackagePrefix;
        m_nativeManager = new NativeScreenManager( );
        m_screens = new Map<ScreenID, Screen>( );

        m_screensContainer = js.Browser.document.querySelector( '#screens' );

        Application.broadcastManager.getChannel( 'ScreenManager' )
            .on( 'Event', onScreenEvent )
            .on( 'Entered', onScreenEntered )
            .on( 'Exited', onScreenExited );

        Application.broadcastManager.getChannel( 'GamepadManager' )
            .on( GamepadEventType.ButtonDown, onGamepadBtnDown )
            .on( GamepadEventType.ButtonUp, onGamepadBtnUp )
            .on( GamepadEventType.Connected, onGamepadConnected )
            .on( GamepadEventType.Disconnected, onGamepadDisconnected );

        Application.broadcastManager.getChannel( 'KeyboardManager' )
            .on( KeyboardEventType.KeyDown, onKeyDown )
            .on( KeyboardEventType.KeyUp, onKeyUp );
    }

    public function getScreen(id : ScreenID) : Screen {
        return m_screens[ id ];
    }

    public function removeScreen(screen : Screen) {
        var id = screen.getID( );

        m_nativeManager.removeScreen( id );

        m_screensContainer.removeChild( screen.getFrame( ) );

        m_screens.remove( id );
    }

    public function setScreen(name : String, data : Dynamic) {
        m_nativeManager.setScreen( name, data );
    }

    public function addOverlay(name : String, data : Dynamic) {
        m_nativeManager.addOverlay( name, data );
    }

    private function createScreen(name : String, id : ScreenID, data : Dynamic) {
        var frame = js.Browser.document.createIFrameElement( );

        frame.src = 'Screens/${name}.html';
        frame.style.display = 'none';

        m_screensContainer.appendChild( frame );

        frame.contentWindow.addEventListener( 'load', function() {
            frame.style.display = 'block';

            var screenType = Type.resolveClass( m_screenPackagePrefix + name );

            if (screenType == null)
                throw 'Unknown screen type "${name}".';

            m_screens.set( id, Type.createInstance( screenType, [ id, frame, data ] ) );
        } );
    }

    private function onScreenEvent(e : Dynamic) {
        var screen = m_screens[ e.screenID ];

        if (screen != null)
            screen.events.trigger( e.event, e.data );
    }

    private function onScreenEntered(e : Dynamic) {
        createScreen( e.screenName, e.screenID, e.data );
    }

    private function onScreenExited(e : Dynamic) {
        var screen = m_screens[ e.screenID ];

        if (screen != null)
            screen.exit( );
    }

    private function onGamepadBtnDown(e) {
        var screen = m_screens[ m_nativeManager.getFocusedScreen( ) ];

        if (screen != null)
            screen.events.trigger( GamepadEventType.ButtonUp, e );
    }

    private function onGamepadBtnUp(e) {
        var screen = m_screens[ m_nativeManager.getFocusedScreen( ) ];

        if (screen != null)
            screen.events.trigger( GamepadEventType.ButtonUp, e );
    }

    private function onGamepadConnected(e) {
        var screen = m_screens[ m_nativeManager.getFocusedScreen( ) ];

        if (screen != null)
            screen.events.trigger( GamepadEventType.Connected, e );
    }

    private function onGamepadDisconnected(e) {
        var screen = m_screens[ m_nativeManager.getFocusedScreen( ) ];

        if (screen != null)
            screen.events.trigger( GamepadEventType.Disconnected, e );
    }

    private function onKeyDown(e) {
        var screen = m_screens[ m_nativeManager.getFocusedScreen( ) ];

        if (screen != null)
            screen.events.trigger( KeyboardEventType.KeyDown, e );
    }

    private function onKeyUp(e) {
        var screen = m_screens[ m_nativeManager.getFocusedScreen( ) ];

        if (screen != null)
            screen.events.trigger( KeyboardEventType.KeyUp, e );
    }
}