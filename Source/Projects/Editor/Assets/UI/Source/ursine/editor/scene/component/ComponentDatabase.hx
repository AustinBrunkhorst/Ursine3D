package ursine.editor.scene.component;

typedef NativeEnum = Dynamic<Map<String, Int>>;
typedef NativeMeta = Dynamic<Map<String, Dynamic>>;

extern class ComponentTypeField {
    var type : String;
    var meta : NativeMeta;
    var isEnum : Bool;
}

extern class ComponentType {
    public var meta : NativeMeta;
    public var fields : Dynamic<Map<String, ComponentTypeField>>;
}

class ComponentDatabase {
    private var m_enums : Map<String, NativeEnum>;
    private var m_db : Map<String, ComponentType>;

    public function new(database : Dynamic) {
        m_db = new Map<String, ComponentType>( );

        var components = Reflect.fields( database );

        for (name in components) {
            var component = Reflect.field( components, name );

            m_db.set( name, component );
        }

        trace( m_db );
    }
}