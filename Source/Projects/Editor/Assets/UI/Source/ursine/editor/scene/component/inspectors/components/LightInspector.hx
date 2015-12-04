package ursine.editor.scene.component.inspectors.components;

import ursine.editor.scene.entity.Entity;
import ursine.editor.scene.component.ComponentDatabase;

@componentInspector( "Light" )
class LightInspector extends ComponentInspectionHandler {
    private static var m_lightTypeName = "ursine::ecs::LightType";
    private static var m_lightTypeFieldName = "Type";
    private static var m_lightTypeEnum : NativeEnum = null;

    private static var m_lightTypeDirectional = "Directional";
    private static var m_lightTypePoint = "Point";
    private static var m_lightTypeSpot = "Spot";

    private static var m_typeToFields : Map<UInt, Array<String>> = null;

    private var m_typeFields : Array<FieldInspectionHandler>;

    public function new(entity : Entity, component : ComponentInspection) {
        super( entity, component );

        fieldChangeEvents.on( m_lightTypeFieldName, onLightTypeChange );

        m_typeFields = new Array<FieldInspectionHandler>( );

        if (m_lightTypeEnum == null)
            initTypeToFields( );

        var database = Editor.instance.componentDatabase;

        var typeInstance = Reflect.field( component.value, m_lightTypeFieldName );

        // add type field
        addField(
            database.createFieldInspector(
                this,
                typeInstance,
                database.getComponentTypeField( m_componentType, m_lightTypeFieldName ),
                database.getNativeType( m_lightTypeName )
            )
        );

        setType( typeInstance );
    }

    private function setType(type : UInt) {
        var database = Editor.instance.componentDatabase;

        var componentType = database.getComponentType( m_component.type );

        // remove old fields
        while (m_typeFields.length > 0)
            m_typeFields.pop( ).remove( );

        var fields = m_typeToFields[ type ];

        // add type fields
        for (fieldName in fields) {
            var field = database.getComponentTypeField( m_componentType, fieldName );

            // ignore light type
            if (field.name == m_lightTypeFieldName)
                continue;

            var instance = Reflect.field( m_component.value, field.name );

            var type = database.getNativeType( field.type );

            var inspector = database.createFieldInspector( this, instance, field, type );

            addField( inspector );

            m_typeFields.push( inspector );
        }
    }

    private function onLightTypeChange(e) {
        setType( e.value );
    }

    private function initTypeToFields() {
        m_lightTypeEnum = Editor.instance.componentDatabase.getNativeType( m_lightTypeName ).enumValue;
        m_typeToFields = new Map<UInt, Array<String>>( );

        m_typeToFields[ Reflect.field( m_lightTypeEnum, m_lightTypeDirectional ) ] = [
            "Color",
            "Direction"
        ];

        m_typeToFields[ Reflect.field( m_lightTypeEnum, m_lightTypePoint ) ] = [
            "Color",
            "Intensity",
            "Radius"
        ];

        m_typeToFields[ Reflect.field( m_lightTypeEnum, m_lightTypeSpot ) ] = [
            "Color",
            "Direction",
            "Intensity",
            "SpotlightAngles"
        ];
    }
}
