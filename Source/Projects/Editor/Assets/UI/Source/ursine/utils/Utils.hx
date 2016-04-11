package ursine.utils;

class Utils {
    public static function createMap<T>(object : Dynamic) : Map<String, T> {
        var map = new Map<String, T>( );

        var fields = Reflect.fields( object );

        for (field in fields)
            map[ field ] = Reflect.field( object, field );

        return map;
    }

    public static function formatDuration(milliseconds : Float) : String {
        var hours = Math.floor( milliseconds / (1000 * 60 * 60) );
        var minutes = Math.floor( milliseconds / (1000 * 60) );
        var seconds = Math.floor( milliseconds / 1000 );

        var chunks : Array<String> = [ ];

        if (hours > 0)
            chunks.push( '${hours}hrs' );

        if (minutes > 0)
            chunks.push( '${minutes}mins' );

        if (seconds > 0)
            chunks.push( '${seconds}s' );

        chunks.push( '${milliseconds % 1000}ms' );

        return chunks.join( ' ' );
    }
}
