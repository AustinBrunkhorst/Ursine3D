package ursine.editor.windows;

import ursine.editor.scene.ScenePlayState;
import js.html.HtmlElement;
import js.html.Element;
import js.html.DOMElement;
import ursine.api.input.KeyboardKey;
import ursine.native.Extern;
import ursine.controls.TreeView;
import ursine.controls.TreeViewItem;
import ursine.controls.ContextMenu;
import ursine.editor.scene.entity.EntityEvent;
import ursine.editor.scene.entity.Entity;

class SceneOutline extends WindowHandler {
    public static var instance : SceneOutline;

    private static inline var m_selectedComponent = 'Selected';

    private var m_rootView : TreeView;
    private var m_entityItems : Map<UInt, TreeViewItem>;

    public function new() {
        instance = this;

        super( );

        window.heading = "Outline";
        window.classList.add( 'scene-outline-window' );

        m_rootView = new TreeView( );
        {
            m_rootView.setAsRoot( true );
            m_rootView.enableModification = true;
        }

        m_entityItems = new Map<UInt, TreeViewItem>( );

        window.container.appendChild( m_rootView );

        resetScene( );

        Editor.instance.broadcastManager.getChannel( 'SceneManager' )
            .on( 'WorldChanged', resetScene )
            .on( 'PlayStateChanged', onPlaystateChanged );

        Editor.instance.broadcastManager.getChannel( 'EntityManager' )
            .on( EntityEvent.RefreshEntities, refresh )
            .on( EntityEvent.EntityAdded, onEntityAdded )
            .on( EntityEvent.EntityRemoved, onEntityRemoved )
            .on( EntityEvent.EntityNameChanged, onEntityNameChanged )
            .on( EntityEvent.EntityParentChanged, onEntityParentChanged )
            .on( EntityEvent.ComponentAdded, onComponentAdded )
            .on( EntityEvent.ComponentRemoved, onComponentRemoved );

        window.addEventListener( 'keydown', onWindowKeyDown );
    }

    public function clearSelectedEntities() {
        Extern.SceneClearSelectedEntities( );
    }

    public function deleteSelectedEntities() {
        Extern.SceneDeleteSelectedEntities( );
    }

    private function resetScene() {
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

    private function refresh() {
        m_rootView.innerHTML = '';
        m_entityItems = new Map<UInt, TreeViewItem>( );

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

    private function onWindowKeyDown(e : js.html.KeyboardEvent) {
        switch (e.keyCode) {
            case KeyboardKey.DELETE: {
                deleteSelectedEntities( );
            }
        }
    }

    private function onPlaystateChanged(e) {
        var state = Extern.SceneGetPlayState( );

        m_rootView.classList.toggle( 'inactive', state == ScenePlayState.Playing );

        if (state == ScenePlayState.Paused) {
            refresh( );
        }
    }

    private function onEntityAdded(e) {
        var entity = new Entity( e.uniqueID );

        // race conditions...
        haxe.Timer.delay( function() {
            addEntity( entity );
        }, 0 );
    }

    private function onEntityRemoved(e) {
        var item : TreeViewItem = m_entityItems[ e.uniqueID ];

        if (item == null)
            return;

        // TODO: handle multi selection
        if (untyped item.entity.hasComponent( m_selectedComponent ))
            selectEntity( null );

        if (item.parentNode != null)
            item.parentNode.removeChild( item );

        m_entityItems.remove( e.uniqueID );
    }

    private function onEntityNameChanged(e) {
        var item : TreeViewItem = m_entityItems[ e.uniqueID ];

        if (item == null)
            return;

        item.text = e.name;

        untyped ToolTip.bind( item.textContentElement, e.name );
    }

    private function onEntityParentChanged(e) {
        var item : TreeViewItem = m_entityItems[ e.uniqueID ];

        if (item == null)
            return;

        var entity : Entity = untyped item.entity;

        if (item.parentNode != null)
            item.parentNode.removeChild( item );

        var targetContainer : HtmlElement = null;

        if (e.newParent == null) {
            targetContainer = cast m_rootView;
        } else {
            var newItem : TreeViewItem = m_entityItems[ e.newParent ];

            if (newItem != null)
                targetContainer = cast newItem.child;
        }

        if (targetContainer != null) {
            var children = targetContainer.children;

            if (children.length == 0) {
                targetContainer.appendChild( item );
            } else {
                var index = untyped Math.clamp( entity.getSiblingIndex( ), 0, children.length - 1 );

                targetContainer.insertBefore( item, children[ index ] );
            }
        }
    }

    private function onComponentAdded(e) {
        if (e.component == m_selectedComponent) {
            var item : TreeViewItem = m_entityItems[ e.uniqueID ];

            if (item != null)
                selectEntity( item );
        }
    }

    private function onComponentRemoved(e) {
        if (e.component == m_selectedComponent) {
            var item : TreeViewItem = m_entityItems[ e.uniqueID ];

            if (item != null)
                deselectEntity( item );
        }
    }

    private function addEntity(entity : Entity) {
        // this entity already exists
        if (m_entityItems[ entity.uniqueID ] != null)
            return;

        var item = createEntityItem( entity );

        if (!entity.isVisibleInEditor( ))
            item.classList.add( 'hidden' );

        var parent = entity.getParent( );

        if (parent == null) {
            m_rootView.appendChild( item );
        } else {

            var parentItem : TreeViewItem = m_entityItems[ parent.uniqueID ];

            if (parentItem != null)
                parentItem.child.appendChild( item );
        }

        if (entity.hasComponent( m_selectedComponent ))
            selectEntity( item );
    }

    private function createEntityItem(entity : Entity) : TreeViewItem {
        var item = new TreeViewItem( );

        item.addEventListener( 'contextmenu', function(e) {
            openContextMenu( e, item );

            e.preventDefault( );
            e.stopPropagation( );
            e.stopImmediatePropagation( );
            
            return false;
        } );

        item.addEventListener( 'drag-start', function(e) {
            e.stopPropagation( );
            e.stopImmediatePropagation( );

            e.preventStart = !entity.isHierarchyChangeEnabled( );
        } );

        item.addEventListener( 'drag-drop', function(e) {
            e.stopPropagation( );
            e.stopImmediatePropagation( );

            if (!entity.isHierarchyChangeEnabled( )) {
                e.preventDrop = true;

                return;
            }

            var target : Entity = e.detail.dropTarget.entity;

            var parent = entity.getParent( );

            var targetID = target == null ? -1 : target.uniqueID;
            var parentID = parent == null ? -1 : parent.uniqueID;

            // disable dropping onto the same entity
            if (targetID == parentID) {
                e.preventDrop = e.detail.newParent;

                return;
            }

            // default case
            entity.setParent( target );
        } );

        item.addEventListener( 'drag-drop-after', function(e) {
            e.stopPropagation( );
            e.stopImmediatePropagation( );

            var childIndex = untyped ElementUtils.childIndex( item );

            entity.setSiblingIndex( childIndex );
        } );

        untyped ToolTip.bind( item.textContentElement, entity.getName( ) );

        item.textContentElement.addEventListener( 'dblclick', function(e) {
            startRenamingEntity( item );
        } );

        item.textContentElement.addEventListener( 'keydown', function(e : js.html.KeyboardEvent) {
            if (e.keyCode == KeyboardKey.RETURN) {
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
            // already selected
            if (untyped item.entity.hasComponent( m_selectedComponent ))
                return;

            clearSelectedEntities( );

            untyped item.entity.select( );
        } );

        m_entityItems[ entity.uniqueID ] = item;

        return item;
    }

    private function openContextMenu(e : js.html.MouseEvent, item : TreeViewItem) {
        var menu = new ContextMenu( );

        menu.addItem( 'Rename', function() {
            startRenamingEntity( item );
        } ).icon = 'edit';

        menu.addItem( 'Duplicate', function() {
            var clone : Entity = untyped item.entity.clone( );

            clone.setName( Entity.createCopyName( untyped item.entity.getName( ) ) );
        } ).icon = 'duplicate';

        menu.addSeparator( );

        var delete = menu.addItem( 'Delete', function() {
            untyped item.entity.remove( );
        } );

        delete.icon = 'remove';
        delete.disabled = untyped !item.entity.isRemovalEnabled( );

        menu.open( e.clientX, e.clientY );
    }

    private function startRenamingEntity(item : TreeViewItem) {
        item.textContentElement.contentEditable = 'true';

        var range = js.Browser.document.createRange( );

        range.selectNodeContents( item.textContentElement );

        var selection = js.Browser.window.getSelection( );

        selection.removeAllRanges( );
        selection.addRange( range );
    }

    private function selectEntity(item : TreeViewItem) {
        if (item == null) {
            EntityInspector.instance.inspect( null );
        } else {
            item.selected = true;

            untyped item.scrollIntoViewIfNeeded( );

            // TODO: handle multi selection
            EntityInspector.instance.inspect( untyped item.entity );
        }
    }

    private function deselectEntity(item : TreeViewItem) {
        if (item != null) {
            item.selected = false;

            // TODO: handle multi selection
            EntityInspector.instance.inspect( null );
        }
    }
}
