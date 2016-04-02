package ursine.editor.windows;

import ursine.native.Extern;

import ursine.api.input.KeyboardKey;

import ursine.editor.scene.ScenePlayState;
import ursine.editor.scene.ui.EditorScreenManager;
import ursine.editor.resources.ResourceItem;

import ursine.controls.MenuItem;
import ursine.controls.ItemSelectionPopup;

class SceneView extends NativeCanvasWindowHandler {
    public static var instance : SceneView;

    private static inline var m_resourceTypeArchetype = 'ursine::resources::ArchetypeData';

    private static var m_acceptedResourceDrops = [
        m_resourceTypeArchetype
    ];

    private var m_screenManager : EditorScreenManager;
    private var m_selector : ItemSelectionPopup = null;

    private var m_fullScreen : Bool;
    private var m_fullScreenItem : MenuItem;

    private var m_nonFullScreenContainer : js.html.Element;
    private var m_fullScreenContainer : js.html.Element;

    public function new() {
        instance = this;

        super( 'SceneView' );

        m_screenManager = new EditorScreenManager( window.container );

        window.heading = "Scene";

        window.container.classList.add( 'scene-view-window', 'no-background' );
        window.container.setAttribute( 'accepts-resource-drop', 'true' );
        window.container.addEventListener( 'resource-drag', onResourceDrag );
        window.container.addEventListener( 'resource-drop', onResourceDrop );

        onViewportInvalidated( );

        window.addEventListener( 'resize', onWindowResize );
        window.addEventListener( 'keydown', onWindowKeyDown );

        m_fullScreen = Extern.ProjectGetPreferences( ).fullScreen;

        m_fullScreenItem = Editor.instance.mainMenu
            .findItem( 'View' ).menu
            .findItem( 'Fullscreen Scene' );

        m_nonFullScreenContainer = cast window.container.parentNode;
        m_fullScreenContainer = cast js.Browser.document.querySelector( '#fullscreen-container' );

        // everything is setup by default in non-fullscreen mode
        if (m_fullScreen)
            onFullScreenChanged( );
    }

    public function toggleFullscreen() {
        m_fullScreen = !m_fullScreen;

        Extern.ProjectSetFullScreen( m_fullScreen );

        onFullScreenChanged( );
    }

    public function onFullScreenChanged() {
        m_fullScreenItem.checked = m_fullScreen;

        Editor.instance.toggleStatusBar( !m_fullScreen );

        if (m_fullScreen) {
            Editor.instance.mainDock.style.display = 'none';
            m_nonFullScreenContainer.removeChild( window.container );
            m_fullScreenContainer.appendChild( window.container );

            hookViewportEvents( cast window.container );

            window.container.setAttribute( 'tabindex', '-1' );
            window.container.focus( );
            window.container.addEventListener( 'keydown', onWindowKeyDown );
        } else {
            Editor.instance.mainDock.style.display = 'block';
            m_fullScreenContainer.removeChild( window.container );
            m_nonFullScreenContainer.appendChild( window.container );

            unHookViewportEvents( cast window.container );

            window.container.removeAttribute( 'tabindex' );
            window.container.removeEventListener( 'keydown', onWindowKeyDown );
            window.focus( );
        }

        onViewportInvalidated( );
    }

    private function onWindowResize(e : js.html.CustomEvent) {
        m_screenManager.invalidateScreenViewport( );
    }

    private function onWindowKeyDown(e : js.html.KeyboardEvent) {
        var state = Extern.SceneGetPlayState( );

        // ignore in play mode
        if (state == ScenePlayState.Playing)
            return true;

        switch (e.keyCode) {
            case KeyboardKey.F11: {
                toggleFullscreen( );
            }

            case KeyboardKey.DELETE: {
                SceneOutline.instance.deleteSelectedEntities( );
            }

            case KeyboardKey.SPACE: {
                openEditorCommands( );
            }
        }

        return true;
    }

    private function openEditorCommands() {
        if (m_selector != null) {
            return;
        }

        var items = Extern.GetEditorCommands( );

        m_selector = new ItemSelectionPopup( items );

        m_selector.addEventListener( 'item-selected', function(e) {
            // invoke the command
            untyped __js__( "eval('editor_commands_'+ e.detail.item +'()')" );

            clearCommandSelector( );
        } );

        m_selector.filterInput.addEventListener( 'keydown', function(e) {
            if (e.keyCode == KeyboardKey.SPACE && m_selector.currentFilter.length == 0) {
                m_selector.close( );

                e.preventDefault( );
                e.stopPropagation( );
                e.stopImmediatePropagation( );

                return false;
            }

            return true;
        } );

        m_selector.addEventListener( 'closed', clearCommandSelector );

        // delay so that the spacebar doesn't effect the input
        haxe.Timer.delay( function() {
            if (m_selector == null)
                return;

            js.Browser.document.body.appendChild( m_selector );

            var bounds = window.container.getBoundingClientRect( );

            m_selector.show( bounds.left + (bounds.width - 225) * 0.5, bounds.height * 0.35 );
        }, 10 );
    }

    private function clearCommandSelector() {
        m_selector = null;

        haxe.Timer.delay( window.focus, 1 );
    }

    private function onResourceDrag(e : js.html.CustomEvent) {
        var resource : ResourceItem = e.detail.resource;

        e.detail.acceptDrop = m_acceptedResourceDrops.indexOf( resource.type ) != -1;
    }

    private function onResourceDrop(e : js.html.CustomEvent) {
        var resource : ResourceItem = e.detail.resource;

        switch (resource.type) {
        case m_resourceTypeArchetype:
            Extern.SceneInstantiateArchetype( resource.guid );
        }
    }
}
