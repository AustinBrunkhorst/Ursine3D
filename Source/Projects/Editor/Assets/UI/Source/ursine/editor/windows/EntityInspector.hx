package ursine.editor.windows;

import js.html.DOMElement;
import ursine.native.Property;
import ursine.editor.scene.entity.Entity;
import ursine.editor.scene.entity.EntityEvent;
import ursine.editor.scene.component.ComponentDatabase;
import ursine.editor.scene.component.ComponentInspection;
import ursine.editor.scene.component.inspectors.ComponentInspectionHandler;

import js.html.CustomEvent;
import js.html.DivElement;

import ursine.controls.inspection.ComponentTypeSelector;

import ursine.controls.Button;

class EntityInspector extends WindowHandler {
    public static var instance : EntityInspector;

    private var m_inspectedEntity : Entity = null;

    private var m_componentHandlers : Map<String, ComponentInspectionHandler>;

    // containers
    private var m_headerToolbar : DivElement;
    private var m_inspectorsContainer : DivElement;

    // controls
    private var m_btnSaveArchetype : Button;
    private var m_btnCopyEntity : Button;
    private var m_btnAddComponent : Button;

    private var m_openCache : Map<String, Bool>;

    public function new() {
        instance = this;

        super( );

        m_componentHandlers = new Map<String, ComponentInspectionHandler>( );
        m_openCache = new Map<String, Bool>( );

        window.heading = "Inspector";

        initWindow( );
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
            removeInspector( handler );

        m_componentHandlers.remove( e.component );
    }

    private function onInspectedEntityComponentChanged(e) {
        var handler : ComponentInspectionHandler = m_componentHandlers[ e.component ];

        if (handler != null)
            handler.updateField( e.field, e.value );
    }

    private function clearOldInspection() {
        if (m_inspectedEntity != null) {
            m_inspectedEntity.events
                .off( EntityEvent.ComponentAdded, onInspectedEntityComponentAdded )
                .off( EntityEvent.ComponentRemoved, onInspectedEntityComponentRemoved )
                .off( EntityEvent.ComponentChanged, onInspectedEntityComponentChanged );
        }

        for (handler in m_componentHandlers)
            removeInspector( handler );

        // reset containers
        m_componentHandlers = new Map<String, ComponentInspectionHandler>( );
    }

    public function initializeInspection() {
        if (m_inspectedEntity == null) {
            m_headerToolbar.style.display = 'none';
            m_btnAddComponent.style.display = 'none';

            return;
        }

        m_headerToolbar.style.display = 'block';
        m_btnAddComponent.style.display = 'block';

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
        // this component was already inspected
        if (m_componentHandlers[ component.type ] != null)
            return;

        var database = Editor.instance.componentDatabase;

        var type = database.getComponentType( component.type );

        // skip components marked hidden in inspector
        if (Reflect.hasField( type.meta, Property.HiddenInInspector ))
            return;

        var handler = database.createComponentInspector( m_inspectedEntity, component );

        handler.inspector.canRemove = !Reflect.hasField(
            type.meta,
            Property.DisableComponentRemoval
        );

        // remember opened state for components
        handler.inspector.opened = (m_openCache[ component.type ] == true);

        handler.inspector.addEventListener(
            'removed',
            onRemoveComponentClicked.bind( component )
        );

        handler.inspector.addEventListener(
            'open-changed',
            onOpenChanged.bind( component )
        );

        m_componentHandlers[ component.type ] = handler;

        m_inspectorsContainer.appendChild( handler.inspector );
    }

    private function removeInspector(inspector : ComponentInspectionHandler) {
        inspector.remove( );
    }

    private function getAvailableComponentTypes(entity : Entity) : Array<String> {
        var db = Editor.instance.componentDatabase;

        return db.getComponentTypes( )
            .filter( function(type) {
                var componentType = db.getComponentType( type );

                var isHidden = Reflect.hasField( componentType.meta, Property.HiddenInInspector );

                // entity must not already have this component and this component type must not be
                // explicitly hidden
                return !entity.hasComponent( type ) && !isHidden;
            } );
    }

    private function onArchetypeSaveClicked(e : js.html.MouseEvent) {
        // @@TODO: multi selection handling
        m_inspectedEntity.saveAsArchetype( );
    }

    private function onCopyEntityClicked(e : js.html.MouseEvent) {
        var entity = m_inspectedEntity.clone( );

        entity.setName( m_inspectedEntity.getName( ) + ' Copy' );

        entity.select( );
    }

    private function onAddComponentClicked(e : js.html.MouseEvent) {
        var types = getAvailableComponentTypes( m_inspectedEntity );

        var selector = new ComponentTypeSelector( types );

        selector.style.left = '${e.clientX}px';
        selector.style.top = '${e.clientY}px';

        selector.addEventListener( 'type-selected', onAddComponentTypeSelected );

        js.Browser.document.body.appendChild( selector );
    }

    private function onRemoveComponentClicked(component : ComponentInspection, e : js.html.MouseEvent) {
        m_inspectedEntity.removeComponent( component.type );

        e.stopPropagation( );
    }

    private function onAddComponentTypeSelected(e : CustomEvent) {
        var componentType = e.detail.type;

        if (m_inspectedEntity.hasComponent( componentType )) {
            // TODO: create error dialog
            throw 'Entity already has component type ${componentType}';
        }

        m_inspectedEntity.addComponent( componentType );
    }

    private function onOpenChanged(component : ComponentInspection, e : CustomEvent) {
        m_openCache[ component.type ] = e.detail.open;
    }

    private function initWindow() {
        window.classList.add( 'entity-inspector-window' );

        // Header Toolbar
        m_headerToolbar = js.Browser.document.createDivElement( );
        {
            m_headerToolbar.classList.add( 'header-toolbar' );

            window.container.appendChild( m_headerToolbar );

            // Button Save Archetype
            m_btnSaveArchetype = new Button( );
            {
                m_btnSaveArchetype.classList.add( 'save-archetype' );

                m_btnSaveArchetype.addEventListener( 'click', onArchetypeSaveClicked );

                m_headerToolbar.appendChild( m_btnSaveArchetype );
            }

            // Button Copy Archetype
            m_btnCopyEntity = new Button( );
            {
                m_btnCopyEntity.classList.add( 'copy-entity' );

                m_btnCopyEntity.addEventListener( 'click', onCopyEntityClicked );

                m_headerToolbar.appendChild( m_btnCopyEntity );
            }

            // hidden initially
            m_headerToolbar.style.display = 'none';
        }

        // Inspectors container
        m_inspectorsContainer = js.Browser.document.createDivElement( );
        {
            window.container.appendChild( m_inspectorsContainer );
        }

        // Button Add Component
        m_btnAddComponent = new Button( );
        {
            m_btnAddComponent.text = 'Add Component';

            m_btnAddComponent.classList.add( 'add-component' );

            // hidden initially
            m_btnAddComponent.style.display = 'none';

            m_btnAddComponent.addEventListener( 'click', onAddComponentClicked );

            window.container.appendChild( m_btnAddComponent );
        }
    }
}
