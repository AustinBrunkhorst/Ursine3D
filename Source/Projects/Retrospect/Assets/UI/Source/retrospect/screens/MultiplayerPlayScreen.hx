package retrospect.screens;

import ursine.screen.Screen;

import ursine.input.KeyboardManager;
import ursine.input.KeyboardKey;

import ursine.input.GamepadManager;
import ursine.input.GamepadButton;

class MultiplayerPlayScreen extends Screen {
    private var m_kills : Map<Int, Int>;

    public function new(id : ScreenID, frame : js.html.IFrameElement, data : Dynamic) {
        super( id, frame, data );

        m_kills = new Map<Int, Int>( );

        m_kills[ 1 ] = 0;
        m_kills[ 2 ] = 0;

        events
            .on( KeyboardEventType.KeyDown, onKeyboardKeyDown )
            .on( GamepadEventType.ButtonDown, onGamepadButtonDown )
            .on( 'PlayerDied', onPlayerDied )
            .on( 'PlayerDamageTaken', onPlayerDamageTaken )
            .on( 'RoundReset', onRoundReset );

        Application.screenManager.addOverlay( 'RoundOverlayScreen', {
            title: 'Round 1'
        } );
    }

    private function triggerPause() {
        Application.screenManager.addOverlay( 'PauseScreen', { } );
    }

    private function onKeyboardKeyDown(e : KeyboardKeyEvent) {
        if (!(e.triggered && e.pressed))
            return;

        if (e.key == KeyboardKey.ESCAPE) {
            triggerPause( );
        }
    }

    private function onGamepadButtonDown(e : GamepadButtonEvent) {
        if (!(e.triggered && e.pressed))
            return;

        if (e.button == GamepadButton.BACK) {
            triggerPause( );
        }
    }

    private function onPlayerDied(data : Dynamic) {
        setPlayerHealth( data.player, 0 );
        givePlayerKill( data.player == 1 ? 2 : 1 );
    }

    private function onPlayerDamageTaken(data : Dynamic) {
        setPlayerHealth( data.player, data.percentage );
    }

    private function onRoundReset(data : Dynamic) {
        haxe.Timer.delay( function() {
            setPlayerHealth( 1, 1 );
            setPlayerHealth( 2, 1 );
        }, 100 );
    }

    private function setPlayerHealth(player : Int, percentage : Float) {
        var container = getPlayerContainer( player );

        container.querySelector( '.health .percentage' ).style.width = Std.int( Math.round( 100 * percentage ) ) + '%';
    }

    private function givePlayerKill(player : Int) {
        var kills = m_kills[ player ] + 1;

        m_kills[ player ] = kills;

        var killsContainer = getPlayerContainer( player ).querySelector( '.kills' );

        killsContainer.classList.remove( 'jello', 'animated' );

        haxe.Timer.delay( function() {
            killsContainer.classList.add( 'jello', 'animated' );
        }, 100 );

        setPlayerKills( player, kills );
    }

    private function setPlayerKills(player : Int, kills : Int) {
        var container = getPlayerContainer( player );

        container.querySelector( '.kills .total' ).innerHTML = cast kills;
    }

    private function getPlayerContainer(player : Int) : js.html.DivElement {
        return cast m_document.querySelector( '.player-${player}' );
    }
}