package ursine.editor.windows;

import ursine.controls.ItemSelectionPopup;
import ursine.native.Extern;

class SceneView extends NativeCanvasWindowHandler {
    private var m_selector : ItemSelectionPopup = null;

    public function new() {
        super( 'SceneView' );

        window.heading = "Scene";

        onViewportInvalidated( );

        window.addEventListener( 'keydown', onWindowKeyDown );
    }

    private function onWindowKeyDown(e : js.html.KeyboardEvent) {
        switch (e.keyCode) {
            // delete
            case 46: {
                SceneOutline.instance.deleteSelectedEntities( );
            }
            // space
            case 32: {
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
            if (e.keyCode == 32 && m_selector.currentFilter.length == 0) {
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
        haxe.Timer.delay(function() {
            js.Browser.document.body.appendChild( m_selector );

            var bounds = window.container.getBoundingClientRect( );

            m_selector.show( bounds.left + (bounds.width - 225) * 0.5, bounds.height * 0.35 );
        }, 10 );
    }

    private function clearCommandSelector() {
        m_selector = null;

        haxe.Timer.delay( window.focus, 1 );
    }
}
