package retrospect.screens;

import ursine.screen.Screen;

import ursine.input.KeyboardManager;
import ursine.input.KeyboardKey;

import ursine.input.GamepadManager;
import ursine.input.GamepadButton;

import ursine.utils.Utils;

class BasicMenuScreen extends Screen {
    public var menu : BasicMenu;
    public var handlers : Map<String, Dynamic>;

    private var m_exiting : Bool;

    public function new(id : ScreenID, frame : js.html.IFrameElement, data : Dynamic) {
        super( id, frame, data );

        menu = new BasicMenu( cast m_document.querySelector( '.basic-menu' ) );
        handlers = new Map<String, Dynamic>( );

        m_exiting = false;

        events
            .on( KeyboardEventType.KeyDown, onKeyboardKeyDown )
            .on( GamepadEventType.ButtonDown, onGamepadButtonDown );
    }

    public function transitionExit(callback : Dynamic, transition : String = "bounceOutUp") : Void {
        m_exiting = true;

        var wrapper : js.html.DivElement = cast menu.container.parentNode;

        wrapper.className = 'inner-wrapper classlist '+ transition;

        ElementUtils.once( wrapper, 'webkitAnimationEnd', function() {
            if (callback != null)
                callback( );

            exit( );
        } );
    }

    private function apply() {
        if (menu.activeItem == null)
            return;

        var handlerName = menu.activeItem.getAttribute( 'data-handler' );

        var handler = handlers[ handlerName ];

        if (handler == null)
            return;

        handler( );
    }

    private function onKeyboardKeyDown(e : KeyboardKeyEvent) {
        if (m_exiting || !(e.triggered && e.pressed))
            return;

        if (e.key == KeyboardKey.RETURN) {
            apply( );
        } else if (e.key == KeyboardKey.UP || e.key == KeyboardKey.W) {
            menu.up( );
        } else if (e.key == KeyboardKey.DOWN  || e.key == KeyboardKey.S) {
            menu.down( );
        }
    }

    private function onGamepadButtonDown(e : GamepadButtonEvent) {
        if (m_exiting || !(e.triggered && e.pressed))
            return;

        if (e.button == GamepadButton.A) {
            apply( );
        } else if (e.button == GamepadButton.DPAD_UP || e.button == GamepadButton.STICK_LEFT_UP) {
            menu.up( );
        } else if (e.button == GamepadButton.DPAD_DOWN || e.button == GamepadButton.STICK_LEFT_DOWN) {
            menu.down( );
        }
    }
}