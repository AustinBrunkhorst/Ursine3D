package ursine.editor.scene.component.inspectors.fields;


import ursine.controls.Button;
import ursine.controls.ContextMenu;
import ursine.controls.inspection.FieldInspector;
import ursine.controls.inspection.ArrayItemContainer;

import ursine.editor.scene.component.ComponentDatabase;

class ArrayTypeInspector implements IFieldInspectionOwner {
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

        updateValue( instance );
    }

    public function updateValue(value : Dynamic)  {
        m_itemsContainer.innerHTML = '';
        m_arrayItems = [ ];

        for (i in 0 ... untyped value.length) {
            arrayInsert( i, value[ i ] );

            m_arrayItems[ i ].opened = false;
        }
    }

    public function arrayInsert(index : UInt, value : Dynamic) {
        var handler = inspectItem( value );
        var container = new ArrayItemContainer( );

        handler.arrayIndex = index;

        container.opened = true;
        container.container.appendChild( handler.inspector );
        container.handler = handler;

        container.header.addEventListener( 'contextmenu', function(e) {
            openItemContextMenu( e, handler, container );

            e.preventDefault( );
            e.stopPropagation( );
            e.stopImmediatePropagation( );
        } );

        container.addEventListener( 'item-removed', function() {
            m_owner.entity.componentFieldArrayRemove(
                m_owner.component.type,
                m_field.name,
                handler.arrayIndex
            );
        } );

        m_arrayItems.insert( index, container );

        // adding to the end
        if (index == Math.max( 0, m_arrayItems.length - 1 )) {
            m_itemsContainer.appendChild( container );

            container.index = index;
        } else {
            var item = m_arrayItems[ index + 1 ];

            m_itemsContainer.insertBefore( item, container );

            // update index stuff
            for (i in index...m_arrayItems.length) {
                var element = m_arrayItems[ i ];

                element.index = i;
            }
        }
    }

    public function arraySet(index : UInt, value : Dynamic) {
        var container = m_arrayItems[ index ];

        if (container == null)
            throw 'Invalid array index.';

        container.handler.updateValue( value );
    }

    public function arrayRemove(index : UInt) {
        var container = m_arrayItems[ index ];

        if (container == null)
            throw 'Invalid array index.';

        m_arrayItems.splice( index, 1 );

        m_itemsContainer.removeChild( container );

        // update index stuff
        for (i in index...m_arrayItems.length) {
            var element = m_arrayItems[ i ];

            element.index = i;
            element.handler.arrayIndex = i;
        }
    }

    private function initElements() {
        m_parent.classList.add( 'array-owner' );

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
            m_btnAddItem.text = 'Add ${m_type.arrayType}';

            m_btnAddItem.addEventListener( 'click', onAddItemClicked );

            itemsWrapper.appendChild( m_btnAddItem );
        }
    }

    public function getFieldHandlers() : Array<FieldInspectionHandler> {
        var handlers = m_owner.getFieldHandlers( );

        for (item in m_arrayItems)
            handlers.push( item.handler );

        return handlers;
    }

    public function ownerNotifyChanged(handler : FieldInspectionHandler, field : NativeField, value : Dynamic) {
        m_owner.entity.componentFieldArrayUpdate(
            m_owner.component.type,
            field.name,
            handler.arrayIndex,
            value
        );
    }

    private function inspectItem(value) : FieldInspectionHandler {
        var database = Editor.instance.componentDatabase;

        var arrayType = database.getNativeType( m_type.arrayType );

        return database.createFieldInspector( this, value, m_field, arrayType );
    }

    private function openItemContextMenu(e : js.html.MouseEvent, handler : FieldInspectionHandler, container : ArrayItemContainer) {
        var menu = new ContextMenu( );

        var moveUp = menu.addItem( 'Move Up', function() {
            m_owner.entity.componentFieldArraySwap(
                m_owner.component.type,
                m_field.name,
                handler.arrayIndex,
                handler.arrayIndex - 1
            );
        } );

        moveUp.icon = 'arrow-up';
        moveUp.disabled = (handler.arrayIndex == 0);

        var moveDown = menu.addItem( 'Move Down', function() {
            m_owner.entity.componentFieldArraySwap(
                m_owner.component.type,
                m_field.name,
                handler.arrayIndex,
                handler.arrayIndex + 1
            );
        } );

        moveDown.icon = 'arrow-down';
        moveDown.disabled = (handler.arrayIndex == m_arrayItems.length - 1);

        menu.addSeparator( );

        var delete = menu.addItem( 'Delete', function() {
            m_owner.entity.componentFieldArrayRemove(
                m_owner.component.type,
                m_field.name,
                handler.arrayIndex
            );
        } );

        delete.icon = 'remove';

        menu.open( e.clientX, e.clientY );
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
