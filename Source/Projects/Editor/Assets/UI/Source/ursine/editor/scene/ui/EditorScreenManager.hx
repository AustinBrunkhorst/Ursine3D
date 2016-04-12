package ursine.editor.scene.ui;

import ursine.native.Extern;

import ursine.editor.resources.ResourceItem;

import ursine.api.ui.Screen;
import ursine.api.ui.ScreenManager;
import ursine.api.input.GamepadManager;
import ursine.api.input.KeyboardManager;
import ursine.api.timers.TimerManager;

import js.html.LinkElement;

@:enum
abstract ConfigReadyState(Int) {
    var Loading = 0;
    var Loaded = 1;
}

class ScreenLayoutCache {
    public var link : LinkElement;
    public var template : js.html.TemplateElement;
    public var logicHandlerType : Dynamic;

    public function new() { }
}

class EditorScreenManager implements ursine.api.ui.ScreenManager {
    public static var instance : ScreenManager;

    // native type for UI project resources
    private static inline var m_uiProjectResourceType = 'ursine::resources::UIProjectData';

    // HTML attribute that declares what class is used for the screen logic
    private static inline var m_screenLogicMetaAttribute = 'data-screen-logic';

    // prefix for ui resource urls
    private static inline var m_uiResourcePrefix = 'http://game';

    // path to the project configuration script
    private static inline var m_projectConfigScript = 'UIProject.js';

    private var m_nativeManager : NativeScreenManager;
    private var m_screens : Map<ScreenID, Screen>;

    private var m_container : js.html.DOMElement;

    private var m_projectCacheState : Map<String, ConfigReadyState>;
    private var m_projectScriptCache : Map<String, js.html.ScriptElement>;

    private var m_screenLoadQueue : Map<String, Array<Dynamic>>;
    private var m_screenLayoutCache : Map<String, ScreenLayoutCache>;
    private var m_screenTypeCache : Map<String, Class<Dynamic>>;

    private var m_lastPlayState : ScenePlayState;
    private var m_stepTimer : haxe.Timer;

    public function new(container : js.html.DOMElement) {
        instance = this;

        m_nativeManager = new NativeScreenManager( );
        m_screens = new Map<ScreenID, Screen>( );

        m_container = js.Browser.document.createDivElement( );
        {
            m_container.classList.add( 'screen-manager' );

            container.appendChild( m_container );
        }

        m_projectCacheState = new Map<String, ConfigReadyState>( );
        m_projectScriptCache = new Map<String, js.html.ScriptElement>( );

        m_screenLoadQueue = new Map<String, Array<Dynamic>>( );
        m_screenLayoutCache = new Map<String, ScreenLayoutCache>( );
        m_screenTypeCache = new Map<String, Class<Dynamic>>( );

        m_lastPlayState = ScenePlayState.InEditor;
        m_stepTimer = null;

        var kbManager = new KeyboardManager( );
        var gpManager = new GamepadManager( );

        var bm = Editor.instance.broadcastManager;

        bm.getChannel( 'ResourcePipeline' )
            .on( 'ResourceModified', onResourceModified );

        bm.getChannel( 'SceneManager' )
            .on( 'PlayStateChanged', onScenePlayStateChanged )
            .on( 'FrameStepped', onSceneFrameStepped );

        bm.getChannel( 'ScreenManager' )
            .on( 'ScreenAdded', onScreenAdded )
            .on( 'ScreenMessaged', onScreenMessaged )
            .on( 'ScreenExited', onScreenExited )
            .on( 'ScreensCleared', onScreensCleared );

        bm.getChannel( 'GamepadManager' )
            .on( GamepadEventType.ButtonDown, onGamepadBtnDown )
            .on( GamepadEventType.ButtonUp, onGamepadBtnUp )
            .on( GamepadEventType.Connected, onGamepadConnected )
            .on( GamepadEventType.Disconnected, onGamepadDisconnected );

        bm.getChannel( 'KeyboardManager' )
            .on( KeyboardEventType.KeyDown, onKeyDown )
            .on( KeyboardEventType.KeyUp, onKeyUp );

        onScenePlayStateChanged( );
    }

    public function getScreen(id : ScreenID) : Screen {
        return m_screens[ id ];
    }

    public function removeScreen(screen : Screen) : Void {
        var id = screen.getID( );

        m_nativeManager.removeScreen( id );

        m_container.removeChild( screen.getHost( ) );

        m_screens.remove( id );
    }

    public function addScreen(path : String, initData : Dynamic, inputBlocking : Bool = true, priority : Int = 0) : ScreenID {
        var id = m_nativeManager.createScreen( path, inputBlocking, priority );

        createScreen( path, id, priority, initData );

        return id;
    }

    public function hasInputFocus(screen : Screen) : Bool {
        return hasFocus( ) && m_nativeManager.screenHasFocus( screen.getID( ) );
    }

    public function messageNativeOwner(screen : Screen, message : String, data : Dynamic) : Void {
        m_nativeManager.messageOwner( screen.getID( ), message, data );
    }

    public function messageNativeGlobal(message : String, data : Dynamic) : Void {
        m_nativeManager.messageGlobal( message, data );
    }

    public function clearScreens() {
        m_container.innerHTML = '';
        m_screens = new Map<ScreenID, Screen>( );
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
        m_projectCacheState[ guid ] = ConfigReadyState.Loading;

        var head = js.Browser.document.head;

        var script = js.Browser.document.createScriptElement( );

        script.src = createQualfiedResourcePath( '${guid}/${m_projectConfigScript}' );

        script.addEventListener( 'load', function() {
            m_projectCacheState[ guid ] = ConfigReadyState.Loaded;

            invalidateScreenTypeCache( );

            initQueuedScreens( guid );
        } );

        script.addEventListener( 'error', function() {
            m_projectCacheState.remove( guid );
            m_projectScriptCache.remove( guid );
            m_screenLoadQueue.remove( guid );

            head.removeChild( script );

            throw 'UI Project "${guid}" is missing "${m_projectConfigScript}" in the root.';
        } );

        m_projectScriptCache[ guid ] = script;

        head.appendChild( script );
    }

    private function initQueuedScreens(guid : String) {
        var queued = m_screenLoadQueue[ guid ];

        if (queued != null) {
            for (callback in queued)
                callback( );

            m_screenLoadQueue.remove( guid );
        }
    }

    private function queueScreen(guid : String, callback : Dynamic) {
        if (m_screenLoadQueue.exists( guid ))
            m_screenLoadQueue.get( guid ).push( callback );
        else
            m_screenLoadQueue.set( guid, [ callback ] );
    }

    private function createScreen(path : String, id : ScreenID, priority : Int, data : Dynamic) {
        // guid of the project this screen belongs to
        var guid = getProjectGUID( path );

        var readyCallback = onScreenElementReady.bind(
            guid,
            createQualfiedResourcePath( path ),
            id,
            priority,
            data
        );

        switch (m_projectCacheState[ guid ]) {
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

    private function cacheScreenLayout(qualifiedPath : String, link : LinkElement) {
        var layout = new ScreenLayoutCache( );

        layout.link = link;
        layout.template = cast link.import_.querySelector( 'template' );

        if (layout.template == null)
            throw 'Screen layout "${qualifiedPath}" missing <template>';

        var styleSheets = link.import_.querySelectorAll( 'link[rel="stylesheet"]' );

        if (styleSheets != null) {
            var styleContainer = js.Browser.document.createStyleElement( );

            for (node in styleSheets) {
                var sheet : LinkElement = cast node;

                styleContainer.innerHTML += '@import url(${sheet.href});';

                node.parentNode.removeChild( node );
            }

            layout.template.content.appendChild( styleContainer );
        }

        var meta : js.html.MetaElement = cast link.import_.querySelector( 'meta' );

        if (meta == null || !meta.hasAttribute( m_screenLogicMetaAttribute ))
            throw 'Screen layout "${qualifiedPath}" missing meta.${m_screenLogicMetaAttribute}';

        var handler = meta.getAttribute( m_screenLogicMetaAttribute );

        layout.logicHandlerType = m_screenTypeCache[ handler ];

        if (layout.logicHandlerType == null)
            throw 'Unknown screen logic handler type "${handler}" in "${qualifiedPath}".';

        m_screenLayoutCache[ qualifiedPath ] = layout;
    }

    private function onScreenElementReady(project : String, qualifiedPath : String, id : ScreenID, priority : Int, data : Dynamic) {
        var cached = m_screenLayoutCache[ qualifiedPath ];

        var readyCallback = onScreenLayoutReady.bind( project, _, id, priority, data );

        if (cached == null) {
            var head = js.Browser.document.head;
            var link = js.Browser.document.createLinkElement( );

            link.rel = 'import';

            // to ensure no cache
            link.href = '${qualifiedPath}?t=${Date.now( ).getTime( )}';

            link.addEventListener( 'load', function() {
                try {
                    cacheScreenLayout( qualifiedPath, link );
                } catch (e : String) {
                    head.removeChild( link );

                    throw 'Unable to construct screen. error: ${e}';
                }

                readyCallback( m_screenLayoutCache[ qualifiedPath ] );
            } );

            link.addEventListener( 'error', function() {
                head.removeChild( link );

                throw 'Failed to load screen layout ${qualifiedPath}';
            } );

            head.appendChild( link );
        } else {
            readyCallback( cached );
        }
    }

    private function onScreenLayoutReady(project : String, layout : ScreenLayoutCache, id : ScreenID, priority : Int, data : Dynamic) {
        var container = js.Browser.document.createDivElement( );

        container.classList.add( 'screen' );
        container.style.zIndex = Std.string( priority );

        var element = container.createShadowRoot( );

        element.appendChild( js.Browser.document.importNode( layout.template.content, true ) );

        m_container.appendChild( container );

        var config : ScreenConfig = {
            project: project,
            owner: this,
            id: id,
            container: element,
            data: data
        };

        var screen : Screen = Type.createInstance( layout.logicHandlerType, [ config ] );

        // make sure it's paused if we're currently paused
        if (Extern.SceneGetPlayState( ) == ScenePlayState.Paused)
            screen.pause( );

        m_screens.set( id, screen );
    }

    private function getProjectGUID(path : String) {
        return haxe.io.Path.normalize( path ).split( '/' )[ 0 ];
    }

    private function invalidateProjectCache(projectGUID : String) {
        if (!m_projectCacheState.exists( projectGUID ))
            return;

        m_projectCacheState.remove( projectGUID );

        var head = js.Browser.document.head;

        var scriptCache = m_projectScriptCache[ projectGUID ];

        if (scriptCache != null && head.contains( scriptCache )) {
            head.removeChild( scriptCache );

            m_projectScriptCache.remove( projectGUID );
        }

        var removalQueue = new Array<String>( );

        for (qualifiedPath in m_screenLayoutCache.keys( )) {
            var path = StringTools.replace( qualifiedPath, m_uiResourcePrefix +'/', '' );

            var guid = getProjectGUID( path );

            if (guid != projectGUID)
                continue;

            var cached : ScreenLayoutCache = m_screenLayoutCache[ qualifiedPath ];

            if (cached != null && head.contains( cached.link )) {
                head.removeChild( cached.link );
            }

            removalQueue.push( qualifiedPath );
        }

        for (removed in removalQueue)
            m_screenLayoutCache.remove( removed );
    }

    private function createQualfiedResourcePath(path : String) {
        return haxe.io.Path.normalize( '${m_uiResourcePrefix}/${path}' );
    }

    private function hasFocus() {
        var playState = Extern.SceneGetPlayState( );

        if (playState != ScenePlayState.Playing)
            return false;

        var focused = js.Browser.document.activeElement;

        return focused != null && (m_container == focused || focused.contains( m_container ));
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

    private function pauseScreens() {
        for (screen in m_screens)
            screen.pause( );
    }

    private function resumeScreens() {
        for (screen in m_screens)
            screen.resume( );
    }

    private function onResourceModified(e : Dynamic) {
        // reloads the cache for all resources belonging to a project
        invalidateProjectCache( e.guid );
    }

    private function onScenePlayStateChanged() {
        var state = Extern.SceneGetPlayState( );

        // nothing changed
        if (state == m_lastPlayState)
            return;

        if (m_stepTimer != null)
            m_stepTimer.stop( );

        switch (state) {
            case ScenePlayState.Playing: {
                m_container.classList.add( 'running' );

                if (m_lastPlayState != ScenePlayState.InEditor)
                    resumeScreens( );
            }

            case ScenePlayState.Paused: {
                m_container.classList.remove( 'running' );

                pauseScreens( );
            }

            case ScenePlayState.InEditor: {
                m_container.classList.remove( 'running' );
            }
        }

        m_lastPlayState = state;
    }

    private function onSceneFrameStepped(e : Dynamic) {
        // ignore when we're not paused
        if (Extern.SceneGetPlayState( ) != ScenePlayState.Paused)
            return;

        resumeScreens( );

        m_stepTimer = haxe.Timer.delay( pauseScreens, cast Math.max( e.dt, 30 ) );
    }

    private function onScreenAdded(e : Dynamic) {
        createScreen( e.path, e.id, e.priority, e.initData );
    }

    private function onScreenMessaged(e : Dynamic) {
        var screen = m_screens[ e.id ];

        if (screen != null)
            screen.events.trigger( e.message, e.data );
    }

    private function onScreensCleared() {
        clearScreens( );
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
