package ursine.api.game;

class GameContext {
    public static inline function GetWindowFullScreen() {
        return untyped __js__( "GameContextGetWindowFullScreen()" );
    }

    public static function SetWindowFullScreen(fullScreen : Bool) {
        return untyped __js__( "GameContextSetWindowFullScreen(fullScreen)" );
    }

    public static function GetVolume(target : String) : Float {
        return untyped __js__( "GameContextGetVolume(target)" );
    }

    public static function SetVolume(target : String, value : Float) {
        return untyped __js__( "GameContextSetVolume(value,target)" );
    }

    public static inline function ExitGame() {
        return untyped __js__( "GameContextExitGame()" );
    }
}

class GameAudioContext {
    public static inline function PlayGlobalEvent(event : String) {
        untyped AudioPlayGlobalEvent( event );
    }

    public static inline function StopGlobalEvent(event : String) {
        untyped AudioStopGlobalEvent( event );
    }
}