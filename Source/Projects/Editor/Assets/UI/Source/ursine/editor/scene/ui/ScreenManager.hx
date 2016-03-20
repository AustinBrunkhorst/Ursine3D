package ursine.editor.scene.ui;

import ursine.api.ui.Screen;
import ursine.api.ui.ScreenManager;
import ursine.api.input.GamepadManager;
import ursine.api.input.KeyboardManager;

import js.html.LinkElement;

@:enum
abstract ConfigReadyState(Int) {
    var Loading = 0;
    var Loaded = 1;
}

@:expose( "ScreenManager" )
class ScreenManager {
    public static var instance : ScreenManager;

    // HTML attribute that declares what class is used for the screen logic
    private static inline var m_screenLogicMetaAttribute = 'data-screen-logic';

    // prefix for ui resource urls
    private static inline var m_uiResourcePrefix = 'http://game';

    // path to the project configuration script
    private static inline var m_projectConfigScript = 'UIProject.js';

    private var m_nativeManager : NativeScreenManager;
    private var m_screens : Map<ScreenID, Screen>;

    private var m_container : js.html.DOMElement;

    private var m_screenTypeCache : Map<String, Class<Dynamic>>;
    private var m_projectCache : Map<String, ConfigReadyState>;
    private var m_projectLayoutCache : Map<String, LinkElement>;

    private var m_screenQueue : Map<String, Array<Dynamic>>;

    public function new(container : js.html.DOMElement) {
        instance = this;

        m_nativeManager = new NativeScreenManager( );
        m_screens = new Map<ScreenID, Screen>( );

        m_container = js.Browser.document.createDivElement( );
        {
            m_container.classList.add( 'screen-manager' );

            container.appendChild( m_container );
        }

        m_screenTypeCache = new Map<String, Class<Dynamic>>( );
        m_projectCache = new Map<String, ConfigReadyState>( );
        m_screenQueue = new Map<String, Array<Dynamic>>( );

        Editor.instance.broadcastManager.getChannel( 'ScreenManager' )
            .on( 'ScreenAdded', onScreenAdded )
            .on( 'ScreenMessaged', onScreenMessaged )
            .on( 'ScreenExited', onScreenExited );

        Editor.instance.broadcastManager.getChannel( 'GamepadManager' )
            .on( GamepadEventType.ButtonDown, onGamepadBtnDown )
            .on( GamepadEventType.ButtonUp, onGamepadBtnUp )
            .on( GamepadEventType.Connected, onGamepadConnected )
            .on( GamepadEventType.Disconnected, onGamepadDisconnected );

        Editor.instance.broadcastManager.getChannel( 'KeyboardManager' )
            .on( KeyboardEventType.KeyDown, onKeyDown )
            .on( KeyboardEventType.KeyUp, onKeyUp );
    }

    public function getScreen(id : ScreenID) : Screen {
        return m_screens[ id ];
    }

    public function removeScreen(screen : Screen) {
        var id = screen.getID( );

        m_nativeManager.removeScreen( id );

        m_container.removeChild( screen.getHost( ) );

        m_screens.remove( id );
    }

    public function addScreen(path : String, data : Dynamic, inputBlocking : Bool, priority : Int) {
        var id = m_nativeManager.createScreen( path, data, inputBlocking, priority );
    }

    public function invalidateScreenViewport() {
        for (screen in m_screens)
            screen.invalidateViewport( );
    }

    private function invalidateScreenTypeCache() {
        // global object that screen types are exported to
        var screens = untyped __js__( 'window.screens' );

        var typeNames = Reflect.fields( screens );

        for (typeName in typeNames)
            m_screenTypeCache.set( typeName, Reflect.field( screens, typeName ) );
    }

    private function initProjectConfig(guid : String) {
        m_projectCache[ guid ] = ConfigReadyState.Loading;

        var head = js.Browser.document.querySelector( 'head' );

        var script = js.Browser.document.createScriptElement( );

        script.src = createQualfiedResourcePath( '${guid}/${m_projectConfigScript}' );

        script.addEventListener( 'load', function() {
            m_projectCache[ guid ] = ConfigReadyState.Loaded;

            invalidateScreenTypeCache( );

            initQueuedScreens( guid );
        } );

        script.addEventListener( 'error', function() {
            m_projectCache.remove( guid );
            m_screenQueue.remove( guid );

            head.removeChild( script );

            throw 'UI Project "${guid}" is missing "${m_projectConfigScript}" in the root.';
        } );

        head.appendChild( script );
    }

    private function initQueuedScreens(guid : String) {
        var queued = m_screenQueue[ guid ];

        if (queued != null) {
            for (callback in queued)
                callback( );

            m_screenQueue.remove( guid );
        }
    }

    private function queueScreen(guid : String, callback : Dynamic) {
        if (m_screenQueue.exists( guid ))
            m_screenQueue.get( guid ).push( callback );
        else
            m_screenQueue.set( guid, [ callback ] );
    }

    private function createScreen(path : String, id : ScreenID, priority : Int, data : Dynamic) {
        // guid of the project this screen belongs to
        var guid = getProjectGUID( path );

        var readyCallback = onScreenElementReady.bind(
            createQualfiedResourcePath( path ),
            id,
            priority,
            data
        );

        switch (m_projectCache[ guid ]) {
            // not loaded
            case null: {
                queueScreen( guid, readyCallback );
                initProjectConfig( guid );
            }

            // loading
            case ConfigReadyState.Loading: {
                queueScreen( guid, readyCallback );
            }

            // all dandy
            case ConfigReadyState.Loaded: {
                readyCallback( );
            }
        }
    }

    private function onScreenElementReady(qualifiedPath : String, id : ScreenID, priority : Int, data : Dynamic) {
        var head = js.Browser.document.querySelector( 'head' );

        var link = js.Browser.document.createLinkElement( );

        link.rel = 'import';
        link.href = qualifiedPath;

        link.addEventListener( 'load', function() {
            var template : js.html.TemplateElement = cast link.import_.querySelector( 'template' );

            if (template == null)
                throw 'Screen layout "${qualifiedPath}" missing <template>';

            var styleSheets = link.import_.querySelectorAll( 'link[rel="stylesheet"]' );

            if (styleSheets != null) {
                var styleContainer = js.Browser.document.createStyleElement( );

                for (node in styleSheets) {
                    var sheet : LinkElement = cast node;

                    styleContainer.innerHTML += '@import url(${sheet.href});';
                }

                template.content.appendChild( styleContainer );
            }

            var container = js.Browser.document.createDivElement( );

            container.classList.add( 'screen' );
            container.style.zIndex = Std.string( priority );

            var element = container.createShadowRoot( );

            element.appendChild( js.Browser.document.importNode( template.content, true ) );

            m_container.appendChild( container );

            var meta = link.import_.querySelector( 'meta' );

            if (meta == null || !meta.hasAttribute( m_screenLogicMetaAttribute ))
                throw 'Screen layout "${qualifiedPath}" missing meta.${m_screenLogicMetaAttribute}';

            var handler = meta.getAttribute( m_screenLogicMetaAttribute );

            var type = m_screenTypeCache[ handler ];

            if (type == null)
                throw 'Unknown screen logic handler type "${handler}" in "${qualifiedPath}".';

            m_screens.set( id, Type.createInstance( type, [ id, element, data ] ) );
        } );

        link.addEventListener( 'error', function() {
            throw 'Failed to load screen layout ${qualifiedPath}';

            head.removeChild( link );
        } );

        head.appendChild( link );
    }

    private function getProjectGUID(path : String) {
        return haxe.io.Path.normalize( path ).split( '/' )[ 0 ];
    }

    private function createQualfiedResourcePath(path : String) {
        return '${m_uiResourcePrefix}/${path}';
    }

    private function hasFocus() {
        var focused = js.Browser.document.activeElement;

        return focused != null && m_container == focused || focused.contains( m_container );
    }

    private function forEachFocusedScreen(callback : Dynamic) {
        // determine if this manager has focus
        if (!hasFocus( ))
            return;

        for (screen in m_screens) {
            if (m_nativeManager.screenHasFocus( screen.getID( ) ))
                callback( screen );
        }
    }

    private function onScreenAdded(e : Dynamic) {
        createScreen( e.path, e.id, e.priority, e.initData );
    }

    private function onScreenMessaged(e : Dynamic) {
        var screen = m_screens[ e.id ];

        if (screen != null)
            screen.events.trigger( e.message, e.data );
    }

    private function onScreenExited(e : Dynamic) {
        var screen = m_screens[ e.id ];

        if (screen != null)
            screen.exit( );
    }

    private function onGamepadBtnDown(e) {
        forEachFocusedScreen( function(screen : Screen) {
            screen.events.trigger( GamepadEventType.ButtonDown, e );
        } );
    }

    private function onGamepadBtnUp(e) {
        forEachFocusedScreen( function(screen : Screen) {
            screen.events.trigger( GamepadEventType.ButtonUp, e );
        } );
    }

    private function onGamepadConnected(e) {
        forEachFocusedScreen( function(screen : Screen) {
            screen.events.trigger( GamepadEventType.Connected, e );
        } );
    }

    private function onGamepadDisconnected(e) {
        forEachFocusedScreen( function(screen : Screen) {
            screen.events.trigger( GamepadEventType.Disconnected, e );
        } );
    }

    private function onKeyDown(e) {
        forEachFocusedScreen( function(screen : Screen) {
            screen.events.trigger( KeyboardEventType.KeyDown, e );
        } );
    }

    private function onKeyUp(e) {
        forEachFocusedScreen( function(screen : Screen) {
            screen.events.trigger( KeyboardEventType.KeyUp, e );
        } );
    }
}
