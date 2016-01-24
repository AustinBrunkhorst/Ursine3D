package ursine.native;

class Extern {
    public static function InitGame() {
        return untyped __js__( "InitGame()" );
    }

    public static function QuitGame() {
        return untyped __js__( "QuitGame()" );
    }

    public static function AudioPlayGlobalEvent(event : String) {
        return untyped __js__( "AudioPlayGlobalEvent( event )" );
    }

    public static function AudioStopGlobalEvent(event : String) {
        return untyped __js__( "AudioStopGlobalEvent( event )" );
    }

    public static function AudioIsGlobalEventPlaying(event : String) {
        return untyped __js__( "AudioIsGlobalEventPlaying( event )" );
    }
}
