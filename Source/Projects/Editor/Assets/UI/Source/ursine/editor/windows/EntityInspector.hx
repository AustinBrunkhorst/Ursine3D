package ursine.editor.windows;

import ursine.editor.scene.component.ComponentInspection;
import ursine.native.Property;
import ursine.editor.scene.entity.Entity;
import ursine.editor.scene.entity.EntityEvent;
import ursine.editor.scene.component.inspectors.ComponentInspectionHandler;

class EntityInspector extends WindowHandler {
    public static var instance : EntityInspector;

    private var m_inspectedEntity : Entity = null;

    private var m_componentHandlers : Map<String, ComponentInspectionHandler>;

    public function new() {
        instance = this;

        super( );

        m_componentHandlers = new Map<String, ComponentInspectionHandler>( );

        window.heading = "Inspector";
    }

    public function inspect(entity : Entity) {
        clearOldInspection( );

        m_inspectedEntity = entity;

        initializeInspection( );
    }

    private function onInspectedEntityComponentAdded(e) {
        var inspection = {
            type: e.component,
            value: e.value
        };

        inspectComponent( cast inspection );
    }

    private function onInspectedEntityComponentRemoved(e) {
        var handler = m_componentHandlers[ e.component ];

        if (handler != null)
            window.container.removeChild( handler.inspector );
    }

    private function onInspectedEntityComponentChanged(e) {
        m_componentHandlers[ e.component ].updateField( e.field, e.value );
    }

    private function clearOldInspection() {
        if (m_inspectedEntity != null) {
            m_inspectedEntity.events
                .off( EntityEvent.ComponentAdded, onInspectedEntityComponentAdded )
                .off( EntityEvent.ComponentRemoved, onInspectedEntityComponentRemoved )
                .off( EntityEvent.ComponentChanged, onInspectedEntityComponentChanged );
        }

        for (handler in m_componentHandlers)
            window.container.removeChild( handler.inspector );

        // reset containers
        m_componentHandlers = new Map<String, ComponentInspectionHandler>( );
    }

    public function initializeInspection() {
        if (m_inspectedEntity == null)
            return;

        m_inspectedEntity.events
            .on( EntityEvent.ComponentAdded, onInspectedEntityComponentAdded )
            .on( EntityEvent.ComponentRemoved, onInspectedEntityComponentRemoved )
            .on( EntityEvent.ComponentChanged, onInspectedEntityComponentChanged );

        var inspection = m_inspectedEntity.inspect( );

        for (component in inspection) {
            inspectComponent( component );
        }
    }

    private function inspectComponent(component : ComponentInspection) {
        var database = Editor.instance.componentDatabase;

        var type = database.getComponentType( component.type );

        // skip components marked hidden in inspector
        if (Reflect.hasField( type.meta, Property.HiddenInInspector ))
            return;

        var handler = database.createComponentInspector( m_inspectedEntity, component );

        m_componentHandlers[ component.type ] = handler;

        window.container.appendChild( handler.inspector );
    }
}
