package ursine.editor.scene.component.inspectors.fields;

import ursine.controls.inspection.ArrayItemContainer;
import ursine.controls.Button;
import ursine.controls.inspection.FieldInspector;

import ursine.editor.scene.component.ComponentDatabase;

class ArrayTypeInspector {
    private var m_parent : FieldInspector;
    private var m_owner : ComponentInspectionHandler;
    private var m_instance : Dynamic;
    private var m_field : NativeField;
    private var m_type : NativeType;

    private var m_itemsContainer : js.html.DivElement;
    private var m_btnAddItem : Button;

    private var m_arrayItems : Array<ArrayItemContainer>;

    public function new(parent : FieldInspector, owner : ComponentInspectionHandler, instance : Dynamic, field : NativeField, type : NativeType) {
        m_parent = parent;
        m_owner = owner;
        m_instance = instance;
        m_field = field;
        m_type = type;

        m_arrayItems = new Array<ArrayItemContainer>( );

        initElements( );
    }

    public function arrayInsert(index : UInt, value : Dynamic) {
        var handler = inspectItem( value );
        var container = new ArrayItemContainer( );

        if (m_arrayItems.length == 0) {
            m_itemsContainer.appendChild( container );

            container.index = 0;
            container.container.appendChild( handler.inspector );

            m_arrayItems.push( container );

            return;
        }

        var item = m_arrayItems[ index - 1 ];

        m_itemsContainer.insertBefore( item, handler.inspector );

        m_arrayItems.insert( index, container );

        // update index stuff
        for (i in index...m_arrayItems.length) {
            var element = m_arrayItems[ i ];

            element.index = i;
        }
    }

    public function arraySet(index : UInt, value : Dynamic) {
        var handler = inspectItem( value );
    }

    public function arrayRemove(index : UInt) {

    }

    private function initElements() {
        var itemsWrapper = js.Browser.document.createDivElement( );
        {
            itemsWrapper.classList.add( 'array-items-wrapper' );

            m_parent.container.appendChild( itemsWrapper );
        }

        m_itemsContainer = js.Browser.document.createDivElement( );
        {
            m_itemsContainer.classList.add( 'array-items' );

            itemsWrapper.appendChild( m_itemsContainer );
        }

        m_btnAddItem = new Button( );
        {
            m_btnAddItem.text = 'Add Item';

            m_btnAddItem.addEventListener( 'click', onAddItemClicked );

            itemsWrapper.appendChild( m_btnAddItem );
        }
    }

    private function inspectItem(value) : FieldInspectionHandler {
        var database = Editor.instance.componentDatabase;

        var type = database.getNativeType( m_field.type );
        var arrayType = database.getNativeType( type.arrayType );

        return database.createFieldInspector( m_owner, value, m_field, arrayType );
    }

    private function onAddItemClicked(e : js.html.MouseEvent) {
        m_owner.entity.componentFieldArrayPush(
            m_owner.component.type,
            m_field.name,
            // use the default value
            { }
        );
    }
}
