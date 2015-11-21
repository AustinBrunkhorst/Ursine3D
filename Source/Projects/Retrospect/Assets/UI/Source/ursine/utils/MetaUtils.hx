package ursine.utils;

import haxe.rtti.Meta;
import haxe.rtti.CType.MetaData;

class MetaUtils {
    public static function getDerivedClasses(baseType : Class<Dynamic>) : Array<Class<Dynamic>> {
        var classTypeNames : Array<String> =
            untyped __js__( "Object.keys( $hxClasses )" );

        var resolvedBaseType = Type.resolveClass(
            Type.getClassName( baseType )
        );

        var derivedTypes : Array<Class<Dynamic>> = [ ];

        for (className in classTypeNames) {
            var classType = Type.resolveClass( className );

            var base = Type.getSuperClass( classType );

            if (base == resolvedBaseType)
                derivedTypes.push( classType );
        }

        return derivedTypes;
    }
}
