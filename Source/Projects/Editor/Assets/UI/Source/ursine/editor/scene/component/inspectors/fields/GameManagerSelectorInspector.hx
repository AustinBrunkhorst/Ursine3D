package ursine.editor.scene.component.inspectors.fields;

import ursine.native.Extern;

import ursine.controls.ItemSelectionPopup;

import ursine.editor.scene.component.ComponentDatabase;

@fieldInspector( "GameManagerSelector" )
class GameManagerSelectorInspector extends FieldInspectionHandler {
    private var m_managers : Array<String>;

    private var m_displayText : js.html.DivElement;

    public function new(owner : IFieldInspectionOwner, instance : Dynamic, field : NativeField, type : NativeType) {
        super( owner, instance, field, type );

        m_managers = Extern.GameContextGetAvailableManagers( );

        m_displayText = js.Browser.document.createDivElement( );
        m_displayText.classList.add( 'game-manager-selector' );

        m_displayText.addEventListener( 'click', function(e) {
            var selector = new ItemSelectionPopup( m_managers );

            selector.addEventListener( 'item-selected', onGameManagerSelected );

            js.Browser.document.body.appendChild( selector );

            selector.show( e.clientX, e.clientY );
        } );

        inspector.container.appendChild( m_displayText );

        updateValue( instance );
    }

    public override function updateValue(value : Dynamic) {
        m_displayText.innerText = value;
        m_displayText.classList.toggle( 'empty', value.length == 0 );

        m_instance = value;
    }

    private function onGameManagerSelected(e) {
        notifyChanged( m_field, e.detail.item );
    }
}
