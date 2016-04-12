package ursine.api.game;

class GameContext {
    public static inline function GetWindowFullScreen() {
        return untyped __js__( "GameContextGetWindowFullScreen()" );
    }

    public static function SetWindowFullScreen(fullScreen : Bool) {
        return untyped __js__( "GameContextSetWindowFullScreen(fullScreen)" );
    }

    public static inline function IsMusicMuted() {
        return untyped __js__( "GameContextIsMusicMuted()" );
    }

    public static function MuteMusic(muted : Bool) {
        return untyped __js__( "GameContextMuteMusic(muted)" );
    }

    public static inline function IsAudioMuted() {
        return untyped __js__( "GameContextIsAudioMuted()" );
    }

    public static function MuteAudio(muted : Bool) {
        return untyped __js__( "GameContextMuteAudio(muted)" );
    }

    public static inline function ExitGame() {
        return untyped __js__( "GameContextExitGame()" );
    }
}

class GameAudioContext {
    public static inline function PlayGlobalEvent(event : String) {
        untyped AudioPlayGlobalEvent( event );
    }
}