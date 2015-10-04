package ursine.editor.windows;

import ursine.editor.scene.Entity;

class EntityInspector extends WindowHandler {
    public static var instance : EntityInspector;

    private var m_inspectedEntity : Entity = null;

    private var m_componentContainers : Map<String, ComponentInspectionHandler>;

    public function new() {
        instance = this;

        super( );

        m_componentContainers = new Map<String, ComponentInspectionHandler>( );

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
        trace( 'changed!!!!' );
        trace( e );
    }

    private function clearOldInspection() {
        if (m_inspectedEntity != null)
            m_inspectedEntity.events.off( 'componentChanged', onInspectedEntityComponentChanged );

        for (container in m_componentContainers)
            window.container.removeChild( container );

        // reset containers
        m_componentContainers = new Map<String, ComponentInspectionHandler>( );
    }

    public function initializeInspection() {
        m_inspectedEntity.events.on( 'componentChanged', onInspectedEntityComponentChanged );
    }
}
