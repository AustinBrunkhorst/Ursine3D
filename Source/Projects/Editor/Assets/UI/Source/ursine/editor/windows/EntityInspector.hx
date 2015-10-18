package ursine.editor.windows;

import ursine.editor.scene.entity.Entity;
import ursine.editor.scene.entity.EntityEvent;

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
    }
}
