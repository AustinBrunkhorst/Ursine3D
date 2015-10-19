package ursine.editor.scene.component;

typedef NativeMeta = Dynamic<Map<String, Dynamic>>;
typedef NativeEnum = Dynamic<Map<String, Dynamic>>;

extern class NativeType {
    var type : String;
}

extern class ComponentTypeField {
    var type : String;
    var isEnum : Bool;
    var enumValue : NativeEnum;

    var meta : NativeMeta;
}

extern class ComponentType {
    public var meta : NativeMeta;
    public var fields : Dynamic<Map<String, ComponentTypeField>>;
}

class ComponentDatabase {
    private var m_typeDB : Map<String, NativeType>;
    private var m_db : Map<String, ComponentType>;

    public function new(database : Dynamic) {
        m_db = new Map<String, ComponentType>( );

        var components = Reflect.fields( database );

        for (name in components) {
            var component = Reflect.field( database, name );

            m_db.set( name, component );
        }

        trace( m_db );
    }
}