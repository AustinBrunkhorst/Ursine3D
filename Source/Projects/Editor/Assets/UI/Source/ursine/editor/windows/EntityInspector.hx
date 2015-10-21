package ursine.editor.windows;

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

        window.style.top = "0";
        window.style.bottom = "50%";
        window.style.left = "0";
        window.style.width = "15%";

        // TODO:
        window.style.borderBottom = "none";
    }

    public function inspect(entity : Entity) {
        clearOldInspection( );

        m_inspectedEntity = entity;

        initializeInspection( );
    }

    private function onInspectedEntityComponentChanged(e) {
        m_componentHandlers[ e.component ].updateField( e.field, e.value );
    }

    private function clearOldInspection() {
        if (m_inspectedEntity != null)
            m_inspectedEntity.events.off( EntityEvent.ComponentChanged, onInspectedEntityComponentChanged );

        for (handler in m_componentHandlers)
            window.container.removeChild( handler.inspector );

        // reset containers
        m_componentHandlers = new Map<String, ComponentInspectionHandler>( );
    }

    public function initializeInspection() {
        m_inspectedEntity.events.on( EntityEvent.ComponentChanged, onInspectedEntityComponentChanged );

        var inspection = m_inspectedEntity.inspect( );

        for (component in inspection) {
            var handler =
                Editor.instance.componentDatabase.createComponentInspector( m_inspectedEntity, component );

            m_componentHandlers[ component.type ] = handler;

            window.container.appendChild( handler.inspector );
        }
    }
}
