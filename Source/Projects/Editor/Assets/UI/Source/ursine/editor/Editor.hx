package ursine.editor;

import ursine.editor.resources.NativeInstallManager;
import haxe.rtti.Meta;
import ursine.api.native.NativeBroadcastManager;
import ursine.native.Extern;
import ursine.controls.Menu;
import ursine.controls.MainMenu;
import ursine.controls.MenuItem;
import ursine.controls.MenuSeparator;
import ursine.controls.docking.DockContainer;
import ursine.editor.MenuItemHandler;
import ursine.editor.windows.OutputLog;
import ursine.editor.resources.NativeResourceManager;
import ursine.editor.resources.NativeBuildManager;
import ursine.editor.scene.component.ComponentDatabase;
import ursine.editor.scene.ScenePlayState;

import ursine.editor.menus.MainMenus;

class Editor {
    public static var instance : Editor = null;

    public var mainMenu : MainMenu;
    public var mainDock : DockContainer;

    public var outputWindow : OutputLog;

    public var broadcastManager : NativeBroadcastManager;

    public var componentDatabase : ComponentDatabase;

    private var m_notificationManager : NativeNotificationManager;
    private var m_resourceManager : NativeResourceManager;
    private var m_buildManager : NativeBuildManager;
    private var m_installManager : NativeInstallManager;

    private var m_statusTextContainer : js.html.DivElement;

    private var m_toolsContainer : js.html.Element;
    private var m_btnPlay : js.html.Element;
    private var m_btnToggle : js.html.Element;
    private var m_btnStep : js.html.Element;
    private var m_btnStop : js.html.Element;

    public function new() {
        instance = this;

        mainMenu = new MainMenu( );

        broadcastManager = new NativeBroadcastManager( );

        outputWindow = new OutputLog( );

        broadcastManager.getChannel( 'SceneManager' )
            .on( 'PlayStateChanged', onScenePlayStateChanged );

        componentDatabase = new ComponentDatabase(
            Extern.GetNativeComponentDatabase( )
        );

        buildMenus( );

        m_notificationManager = new NativeNotificationManager( broadcastManager );
        m_resourceManager = new NativeResourceManager( broadcastManager );
        m_buildManager = new NativeBuildManager( broadcastManager );
        m_installManager = new NativeInstallManager( broadcastManager );

        m_statusTextContainer = cast js.Browser.document.querySelector( '#status-bar span' );

        js.Browser.document
            .querySelector( '#header-toolbar' )
            .appendChild( mainMenu );

        initSimulationPlayback( );

        onScenePlayStateChanged( );
    }

    public function setStatusText(status : String) {
        m_statusTextContainer.innerHTML = status;
    }

    public function toggleStatusBar(visible : Bool) {
        untyped m_statusTextContainer.parentNode.style.display = visible ? 'block' : 'none';
    }

    private function buildMenus() {
        var classTypeNames : Array<String> =
            untyped __js__( "Object.keys( $hxClasses )" );

        var menuHandlerType = Type.resolveClass(
            Type.getClassName( MenuItemHandler )
        );

        var handlers : Array<Dynamic> = [ ];

        for (className in classTypeNames) {
            var classType = Type.resolveClass( className );

            var base = Type.getSuperClass( classType );

            if (base == menuHandlerType) {
                var info : Dynamic = { };

                info.type = classType;

                var meta = Meta.getType( classType );

                if (Reflect.hasField( meta, "menuIndex" ))
                    info.index = meta.menuIndex[ 0 ];
                else
                    info.index = Math.POSITIVE_INFINITY;

                handlers.push( info );
            }
        }

        // sort based on index ascending order
        handlers.sort( function(a, b) {
            return a.index - b.index;
        } );

        for (handler in handlers) {
            var statics = Meta.getStatics( handler.type );
            var fields = Reflect.fields( statics );

            for (name in fields) {
                var field = Reflect.field( statics, name );

                // isn't a menu item
                if (!Reflect.hasField( field, "mainMenuItem" ))
                    continue;

                var details : Array<Dynamic> = field.mainMenuItem;

                // levels in the menu
                var levels = StringTools
                    .replace( details[ 0 ], '"', "" )
                    .split( "/" );

                // clickable item that we're creating
                var itemName = levels.pop( );

                // parent menu that this item will be added to
                var parentMenu = populateMenuLevels( levels );

                var item = new MenuItem( );

                item.text = itemName;
                item.icon = details[ 3 ];

                var callback = Reflect.field( handler.type, name );

                item.addEventListener( 'open', function(e) {
                    Reflect.callMethod( null, cast callback, [ e ] );
                } );

                // separator before
                if (details[ 1 ] == true) {
                    parentMenu.appendChild( new MenuSeparator( ) );
                }

                parentMenu.appendChild( item );

                // separator after
                if (details[ 2 ] == true) {
                    parentMenu.appendChild( new MenuSeparator( ) );
                }
            }

            var init = Reflect.field( handler.type, 'init' );

            if (init != null)
                Reflect.callMethod( null, cast init, [ mainMenu ] );
        }
    }

    private function populateMenuLevels(levels : Array<String>) : Menu {
        var parent : Menu = mainMenu;

        while (levels.length > 0) {
            var level = levels.shift( );

            var item : MenuItem = parent.findItem( level );

            // item doesn't exist, create it
            if (item == null) {
                item = new MenuItem( );

                item.text = level;

                parent.appendChild( item );
            }

            parent = item.menu;
        }

        return parent;
    }

    private function initSimulationPlayback() {
        m_toolsContainer = js.Browser.document.querySelector( '#simulation-tools' );

        m_btnPlay = js.Browser.document.querySelector( '#simulation-play' );
        m_btnToggle = js.Browser.document.querySelector( '#simulation-toggle' );
        m_btnStep = js.Browser.document.querySelector( '#simulation-step' );
        m_btnStop = js.Browser.document.querySelector( '#simulation-stop' );

        m_btnPlay.addEventListener( 'click', function() {
            Extern.SceneSetPlayState( ScenePlayState.Playing );
        } );

        m_btnToggle.addEventListener( 'click', function() {
            var currentState = Extern.SceneGetPlayState( );

            Extern.SceneSetPlayState( currentState == ScenePlayState.Playing ?
                ScenePlayState.Paused : ScenePlayState.Playing
            );
        } );

        m_btnStep.addEventListener( 'click', function() {
            Extern.SceneStep( );
        } );

        m_btnStop.addEventListener( 'click', function() {
            Extern.SceneSetPlayState( ScenePlayState.InEditor );
        } );
    }

    private function onScenePlayStateChanged() {
        var state = Extern.SceneGetPlayState( );

        switch (state) {
            case ScenePlayState.Playing: {
                m_toolsContainer.classList.add( 'running' );
                m_toolsContainer.classList.remove( 'paused' );
            }

            case ScenePlayState.Paused: {
                m_toolsContainer.classList.add( 'running', 'paused' );
            }

            case ScenePlayState.InEditor: {
                m_toolsContainer.classList.remove( 'running', 'paused' );
            }
        }
    }
}
