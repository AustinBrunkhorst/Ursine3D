package ursine.editor.scene.component.inspectors.fields;

import ursine.native.Extern;
import ursine.native.Property;

import ursine.controls.Notification;
import ursine.controls.ItemSelectionPopup;

import ursine.editor.scene.component.ComponentDatabase;

extern class ResourceReference {
    var guid : String;
    var resolved : Bool;
}

@fieldInspector( "ResourceReference" )
class ResourceReferenceInspector extends FieldInspectionHandler {
    private static var m_nullGUID = '00000000-0000-0000-0000-00000000000';

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

        m_displayText.addEventListener( 'click', function(e) {
            var resources : Array<Dynamic> = Extern.ProjectGetResourcesByType( m_resourceType );

            var items = [ ];

            for (i in 0...resources.length)
                items.push( resources[ i ].relativePathDisplayName );

            var selector = new ItemSelectionPopup( cast items );

            selector.addEventListener( 'item-selected', onResourceSelected );

            js.Browser.document.body.appendChild( selector );

            selector.show( e.clientX, e.clientY );
        } );

        inspector.container.appendChild( m_displayText );

        updateValue( instance );
    }

    public override function updateValue(value : ResourceReference) {
        trace( value );

        //m_displayText.innerText = value.guid;
        m_displayText.classList.toggle( 'invalid', value.resolved == false );

        m_instance = value;
    }

    private function onResourceSelected(e) {
        trace( e.detail.item );
        //notifyChanged( m_field, e.detail.item );
    }
}
