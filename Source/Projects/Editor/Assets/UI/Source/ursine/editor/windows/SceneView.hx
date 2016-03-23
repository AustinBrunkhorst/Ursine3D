package ursine.editor.windows;

import ursine.native.Extern;
import ursine.api.input.KeyboardKey;
import ursine.editor.scene.ui.ScreenManager;
import ursine.editor.resources.ResourceItem;
import ursine.controls.ItemSelectionPopup;

class SceneView extends NativeCanvasWindowHandler {
    private static inline var m_resourceTypeArchetype = 'ursine::resources::ArchetypeData';

    private static var m_acceptedResourceDrops = [
        m_resourceTypeArchetype
    ];

    private var m_screenManager : ScreenManager;
    private var m_selector : ItemSelectionPopup = null;

    public function new() {
        super( 'SceneView' );

        m_screenManager = new ScreenManager( window.container );

        window.heading = "Scene";

        window.container.classList.add( 'scene-view-window' );
        window.container.setAttribute( 'accepts-resource-drop', 'true' );
        window.container.addEventListener( 'resource-drag', onResourceDrag );
        window.container.addEventListener( 'resource-drop', onResourceDrop );

        onViewportInvalidated( );

        window.addEventListener( 'resize', onWindowResize );
        window.addEventListener( 'keydown', onWindowKeyDown );
    }

    private function onWindowResize(e : js.html.CustomEvent) {
        m_screenManager.invalidateScreenViewport( );
    }

    private function onWindowKeyDown(e : js.html.KeyboardEvent) {
        switch (e.keyCode) {
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
