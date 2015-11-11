package ursine.utils;

class Utils {
    public static function createMap<T>(object : Dynamic) : Map<String, T> {
        var map = new Map<String, T>( );

        var fields = Reflect.fields( object );

        for (field in fields)
            map[ field ] = Reflect.field( object, field );

        return map;
    }
}
