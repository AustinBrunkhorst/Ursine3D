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

        var output = '';

        if (hours > 0)
            output += '${hours}hrs';

        if (minutes > 0)
            output += ' ${minutes}mins';

        if (seconds > 0)
            output += ' ${seconds}s';

        if (output.length > 0)
            output += ' ';

        output += '${milliseconds}ms';

        return output;
    }
}
