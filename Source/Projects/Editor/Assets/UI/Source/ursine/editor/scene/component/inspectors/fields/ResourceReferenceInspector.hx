package ursine.editor.scene.component.inspectors.fields;

import ursine.native.Extern;
import ursine.native.Property;

import ursine.controls.ContextMenu;
import ursine.controls.Notification;
import ursine.controls.ResourceReferenceSelectionPopup;

import ursine.editor.resources.ResourceItem;
import ursine.editor.scene.component.ComponentDatabase;

extern class ResourceReference {
    var guid : String;
}

@fieldInspector( "ResourceReference" )
class ResourceReferenceInspector extends FieldInspectionHandler {
    private var m_displayText : js.html.DivElement;
    private var m_resourceType : String;

    public function new(owner : IFieldInspectionOwner, instance : Dynamic, field : NativeField, type : NativeType) {
        super( owner, instance, field, type );

        var resourceType = Reflect.field( field.meta, Property.ResourceType );

        if (resourceType == null) {
            var error = new Notification(
                NotificationType.Error,
                'Field <strong class="highlight">${field.name}</strong> missing meta property <strong class="highlight">ResourceType<strong>',
                'Error'
            );

            error.show( );
        } else {
            m_resourceType = resourceType.typeName;
        }

        m_displayText = js.Browser.document.createDivElement( );
        m_displayText.classList.add( 'resource-reference' );
        m_displayText.setAttribute( 'accepts-resource-drop', 'true' );

        m_displayText.addEventListener( 'click', function(e) {
            var resources : Array<Dynamic> = Extern.ProjectGetResourcesByType( m_resourceType );

            var selector = new ResourceReferenceSelectionPopup( resources );

            selector.addEventListener( 'resource-selected', onResourceSelected );

            js.Browser.document.body.appendChild( selector );

            selector.show( e.clientX, e.clientY );
        } );

        m_displayText.addEventListener( 'contextmenu', function(e) {
            var menu = new ContextMenu( );

            menu.addItem( 'Make Invalid', function() {
                notifyChanged( m_field, {
                    guid: null
                } );
            } ).icon = 'invalid';

            menu.open( e.clientX, e.clientY );
        } );

        m_displayText.addEventListener( 'resource-drag', onResourceDrag );
        m_displayText.addEventListener( 'resource-drop', onResourceDrop );

        inspector.container.appendChild( m_displayText );

        Editor.instance.broadcastManager.getChannel( 'ResourcePipeline' )
            .on( 'ResourceRemoved', onResourceRemoved )
            .on( 'ResourceRenamed', onResourceRenamed );

        updateValue( instance );
    }

    public override function updateValue(value : ResourceReference) {
        var resource = Extern.ProjectGetResource( value.guid );

        var valid = resource != null;

        m_displayText.innerText = valid ? resource.displayName : '';
        m_displayText.classList.toggle( 'invalid', !valid );

        m_instance = value;
    }

    private function onResourceSelected(e : js.html.CustomEvent) {
        var resource : ResourceItem = e.detail.resource;

        notifyChanged( m_field, {
            guid: resource.guid
        } );
    }

    private function onResourceDrag(e : js.html.CustomEvent) {
        var resource : ResourceItem = e.detail.resource;

        e.detail.acceptDrop = resource.type == m_resourceType;
    }

    private function onResourceDrop(e : js.html.CustomEvent) {
        var resource : ResourceItem = e.detail.resource;

        notifyChanged( m_field, {
            guid: resource.guid
        } );
    }

    private function onResourceRemoved(e : Dynamic) {
        var resource : ResourceItem = e.resource;

        // wasn't this resource
        if (resource.guid != m_instance.guid)
            return;

        notifyChanged( m_field, {
            guid: null
        } );
    }

    private function onResourceRenamed(e : Dynamic) {
        var guid : String = e.guid;

        // wasn't this resource
        if (guid != m_instance.guid)
            return;

        var resource : ResourceItem = Extern.ProjectGetResource( guid );

        m_displayText.innerText = resource.displayName;
        m_displayText.classList.toggle( 'invalid', false );
    }
}
