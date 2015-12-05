package ursine.editor.scene.component;


import haxe.rtti.Meta;
import ursine.utils.MetaUtils;
import ursine.native.Property;
import ursine.editor.scene.component.inspectors.fields.DefaultFieldInspector;
import ursine.editor.scene.component.inspectors.components.DefaultComponentInspector;
import ursine.editor.scene.component.inspectors.fields.DefaultFieldInspector;
import ursine.editor.scene.component.inspectors.ComponentInspectionHandler;
import ursine.editor.scene.component.inspectors.FieldInspectionHandler;
import ursine.editor.scene.entity.Entity;

import ursine.editor.scene.component.inspectors.ComponentInspectors;
import ursine.editor.scene.component.inspectors.FieldInspectors;

// Map<String, Dynamic>
typedef NativeMeta = Dynamic;

// Map<String, Dynamic>
typedef NativeEnum = Dynamic;

extern class NativeField {
    var name : String;
    var type : String;
    var meta : NativeMeta;
}

extern class NativeType {
    var name : String;
    var enumValue : Null<NativeEnum>;

    var fields : Array<NativeField>;
}

extern class ComponentType {
    var meta : NativeMeta;

    var fields : Array<NativeField>;
}

class ComponentDatabase {
    private static var m_componentInspectorMeta = "componentInspector";
    private static var m_fieldInspectorMeta = "fieldInspector";

    private var m_componentInspectionHandlers : Map<String, Class<Dynamic>>;
    private var m_fieldInspectionHandlers : Map<String, Class<Dynamic>>;

    private var m_typeDB : Map<String, NativeType>;
    private var m_db : Map<String, ComponentType>;

    public function new(database : Dynamic) {
        m_componentInspectionHandlers = new Map<String, Class<Dynamic>>( );
        m_fieldInspectionHandlers = new Map<String, Class<Dynamic>>( );

        m_typeDB = new Map<String, NativeType>( );
        m_db = new Map<String, ComponentType>( );

        var types = Reflect.fields( database.types );

        for (name in types) {
            var type = Reflect.field( database.types, name );

            m_typeDB.set( name, type );
        }

        var components = Reflect.fields( database.components );

        for (name in components) {
            var component : ComponentType = Reflect.field( database.components, name );

            m_db.set( name, component );
        }

        initComponentInspectors( );
        initFieldInspectors( );
    }

    public function getNativeType(name : String) : NativeType {
        return m_typeDB.get( name );
    }

    public function getComponentTypes() : Array<String> {
        var keys : Array<String> = new Array<String>( );

        for (key in m_db.keys( ))
            keys.push( key );

        return keys;
    }

    public function getComponentType(name : String) : ComponentType {
        return m_db.get( name );
    }

    public function getComponentTypeField(type : ComponentType, name : String) : NativeField {
        return Lambda.find( type.fields, function(f) {
            return f.name == name;
        } );
    }

    public function createComponentInspector(entity : Entity, inspection : ComponentInspection) : ComponentInspectionHandler {
        var handler : Class<Dynamic> = m_componentInspectionHandlers.get( inspection.type );

        if (handler == null)
            handler = DefaultComponentInspector;

        return Type.createInstance( handler, [ entity, inspection ] );
    }

    public function createFieldInspector(owner : ComponentInspectionHandler, instance : Dynamic, field : NativeField, type : NativeType) : FieldInspectionHandler {
        var typeOverride = Reflect.field( field.meta, Property.ForceEditorType );

        var typeName : String;

        if (typeOverride == null) {
            typeName = type.name;
        } else {
            typeName = Reflect.field( typeOverride, 'type' );

            type = m_typeDB[ typeName ];
        }

        var handler = m_fieldInspectionHandlers.get( typeName );

        if (handler == null)
            handler = DefaultFieldInspector;

        return Type.createInstance( handler, [ owner, instance, field, type ] );
    }

    private function initComponentInspectors() {
        var inspectors = MetaUtils.getDerivedClasses( ComponentInspectionHandler );

        for (inspector in inspectors) {
            var meta = Meta.getType( inspector );

            // must have the custom inspector meta data
            if (!Reflect.hasField( meta, m_componentInspectorMeta ))
                continue;

            // names of the component type that this inspector is interested in
            var componentNames : Array<String> = Reflect.field( meta, m_componentInspectorMeta );

            for (name in componentNames) {
                m_componentInspectionHandlers.set( name, inspector );
            }
        }
    }

    private function initFieldInspectors() {
        var inspectors = MetaUtils.getDerivedClasses( FieldInspectionHandler );

        for (inspector in inspectors) {
            var meta = Meta.getType( inspector );

            // must have the custom inspector meta data
            if (!Reflect.hasField( meta, m_fieldInspectorMeta ))
                continue;

            // names of the field type that this inspector is interested in
            var componentNames : Array<String> = Reflect.field( meta, m_fieldInspectorMeta );

            for (name in componentNames) {
                m_fieldInspectionHandlers.set( name, inspector );
            }
        }
    }
}