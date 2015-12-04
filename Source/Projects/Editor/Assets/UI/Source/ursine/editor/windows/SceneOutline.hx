package ursine.editor.windows;

import js.html.HtmlElement;
import js.html.Element;
import js.html.DOMElement;
import js.html.UListElement;
import ursine.native.Extern;
import ursine.controls.TreeView;
import ursine.controls.TreeViewItem;
import ursine.editor.scene.entity.EntityEvent;
import ursine.editor.scene.entity.Entity;

class SceneOutline extends WindowHandler {
    public static var instance : SceneOutline;

    private var m_rootView : TreeView;
    private var m_entityItems : Map<UInt, TreeViewItem>;

    private var m_selectedEntities : Array<UInt> = null;

    public function new() {
        instance = this;

        super( );

        window.heading = "Outline";

        m_rootView = new TreeView( );
        {
            m_rootView.setAsRoot( true );
        }

        m_entityItems = new Map<UInt, TreeViewItem>( );

        m_selectedEntities = new Array<UInt>( );

        window.container.appendChild( m_rootView );

        resetScene( );

        Editor.instance.broadcastManager.getChannel( 'SceneManager' )
            .on( 'Reset', resetScene );

        Editor.instance.broadcastManager.getChannel( 'EntityManager' )
            .on( EntityEvent.EntityAdded, onEntityAdded )
            .on( EntityEvent.EntityRemoved, onEntityRemoved )
            .on( EntityEvent.EntityNameChanged, onEntityNameChanged )
            .on( EntityEvent.EntityParentChanged, onEntityParentChanged )
            .on( EntityEvent.ComponentAdded, onComponentAdded )
            .on( EntityEvent.ComponentRemoved, onComponentRemoved );

        window.addEventListener( 'keydown', onWindowKeyDown );
    }

    public function clearSelectedEntities() {
        for (uid in m_selectedEntities) {
            var item = m_entityItems[ uid ];

            if (item == null)
                continue;

            untyped item.entity.deselect( );
        }
    }

    private function resetScene() {
        m_selectedEntities = new Array<UInt>( );
        m_rootView.innerHTML = '';
        m_entityItems = new Map<UInt, TreeViewItem>( );

        EntityInspector.instance.inspect( null );

        var entities : Array<UInt> = Extern.SceneGetRootEntities( );

        var event = { uniqueID: 0 };

        for (uniqueID in entities) {
            var entity = new Entity( uniqueID );

            initEntity( entity );
        }
    }

    private function initEntity(entity : Entity) {
        addEntity( entity );

        for (child in entity.getChildren( )) {
            initEntity( child );
        }
    }

    private function onWindowKeyDown(e) {
        switch (e.keyCode) {
            // delete
            case 46: {
                deleteSelectedEntities( );
            }
        }
    }

    private function onEntityAdded(e) {
        var entity = new Entity( e.uniqueID );

        addEntity( entity );
    }

    private function onEntityRemoved(e) {
        var item : TreeViewItem = m_entityItems[ e.uniqueID ];

        if (item == null)
            return;

        // TODO: handle multi selection
        if (m_selectedEntities.indexOf( e.uniqueID ) != -1)
            selectEntity( null );

        item.parentNode.removeChild( item );
    }

    private function onEntityNameChanged(e) {
        var item : TreeViewItem = m_entityItems[ e.uniqueID ];

        if (item == null)
            return;

        item.text = e.name;
    }

    private function onEntityParentChanged(e) {
        var item : TreeViewItem = m_entityItems[ e.uniqueID ];

        if (item == null)
            return;

        if (item.parentNode != null)
            item.parentNode.removeChild( item );

        if (e.newParent == null) {
            m_rootView.appendChild( item );
        } else {
            var newItem : TreeViewItem = m_entityItems[ e.newParent ];

            if (newItem != null)
                newItem.child.appendChild( item );
        }
    }

    private function onComponentAdded(e) {
        if (e.component == 'Selected') {
            var item : TreeViewItem = m_entityItems[ e.uniqueID ];

            if (item != null)
                selectEntity( item );
        }
    }

    private function onComponentRemoved(e) {
        if (e.component == 'Selected') {
            var item : TreeViewItem = m_entityItems[ e.uniqueID ];

            if (item != null)
                deselectEntity( item );
        }
    }

    private function addEntity(entity : Entity) {
        if (!entity.isVisibleInEditor( )) {
            m_entityItems[ entity.uniqueID ] = null;
        } else {
            var item = createEntityItem( entity );

            var parent = entity.getParent( );

            if (parent == null) {
                m_rootView.appendChild( item );
            } else {
                var parentItem : TreeViewItem = m_entityItems[ parent.uniqueID ];

                if (parentItem != null)
                    parentItem.child.appendChild( item );
            }

            if (entity.hasComponent( 'Selected' ))
                selectEntity( item );
        }
    }

    private function createEntityItem(entity : Entity) : TreeViewItem {
        var item = new TreeViewItem( );

        item.addEventListener( 'drag-start', function(e) {
            return entity.isHierarchyChangeEnabled( );
        } );

        item.addEventListener( 'drag-drop', function(e) {
            if (!entity.isHierarchyChangeEnabled( ))
                return false;
            
            untyped item.entity.setParent( untyped e.detail.dropTarget.entity );

            // handle manipulation explicitly for new parents
            if (e.detail.newParent == true) {
                return false;
            } else {
                return true;
            }
        } );

        item.textContentElement.addEventListener( 'dblclick', function() {
            item.textContentElement.contentEditable = 'true';

            var range = js.Browser.document.createRange( );

            range.selectNodeContents( item.textContentElement );

            var selection = js.Browser.window.getSelection( );

            selection.removeAllRanges( );
            selection.addRange( range );
        } );

        item.textContentElement.addEventListener( 'keydown', function(e) {
            // return key
            if (e.keyCode == 13) {
                item.textContentElement.blur( );

                e.preventDefault( );

                return false;
            }

            return true;
        } );

        item.textContentElement.addEventListener( 'blur', function() {
            item.textContentElement.contentEditable = 'false';

            entity.setName( item.textContentElement.innerText );
        } );

        item.text = entity.getName( );

        untyped item.entity = entity;

        item.textElement.addEventListener( 'click', function(e) {
            clearSelectedEntities( );

            untyped item.entity.select( );
        } );

        m_entityItems[ entity.uniqueID ] = item;

        return item;
    }

    private function selectEntity(item : TreeViewItem) {
        if (item == null) {
            EntityInspector.instance.inspect( null );
        } else {
            item.selected = true;

            untyped item.scrollIntoViewIfNeeded( );

            // TODO: handle multi selection
            EntityInspector.instance.inspect( untyped item.entity );

            m_selectedEntities.push( untyped item.entity.uniqueID );
        }
    }

    private function deselectEntity(item : TreeViewItem) {
        if (item != null) {
            item.selected = false;

            // TODO: handle multi selection
            EntityInspector.instance.inspect( null );

            m_selectedEntities = m_selectedEntities.filter( function(x) {
                return x != untyped item.entity.uniqueID;
            } );
        }
    }

    private function deleteSelectedEntities() {
        for (uid in m_selectedEntities) {
            var item = m_entityItems[ uid ];

            if (item == null)
                continue;

            var entity : Entity = untyped item.entity;

            if (entity.isRemovalEnabled( )) {
                entity.remove( );
            } else {
                // TODO: add removal warning
            }
        }
    }
}
