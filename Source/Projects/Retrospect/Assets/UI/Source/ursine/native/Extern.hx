package ursine.native;

class Extern {
    public static function InitGame() {
        return untyped __js__( "InitGame()" );
    }
}