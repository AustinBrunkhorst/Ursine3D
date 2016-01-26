package ursine.editor.scene.component.inspectors.fields;

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

    public function new(parent : FieldInspector, owner : ComponentInspectionHandler, instance : Dynamic, field : NativeField, type : NativeType) {
        m_parent = parent;
        m_owner = owner;
        m_instance = instance;
        m_field = field;
        m_type = type;

        initElements( );
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

    private function onAddItemClicked(e : js.html.MouseEvent) {

    }
}
