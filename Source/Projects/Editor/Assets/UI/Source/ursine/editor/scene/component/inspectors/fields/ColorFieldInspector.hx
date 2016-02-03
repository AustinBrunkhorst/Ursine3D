package ursine.editor.scene.component.inspectors.fields;

import ursine.controls.ColorInput;
import ursine.editor.scene.component.ComponentDatabase;

@fieldInspector( "ursine::Color" )
class ColorFieldInspector extends FieldInspectionHandler {
    var m_colorPreview : js.html.DivElement;
    var m_colorInput : ColorInput;

    public function new(owner : IFieldInspectionOwner, instance : Dynamic, field : NativeField, type : NativeType) {
        super( owner, instance, field, type );

        var previewContainer = js.Browser.document.createDivElement( );
        {
            previewContainer.classList.add( 'color-preview' );

            inspector.container.appendChild( previewContainer );
        }

        m_colorPreview = js.Browser.document.createDivElement( );
        {
            m_colorPreview.addEventListener( 'click', onPreviewClick );

            previewContainer.appendChild( m_colorPreview );
        }

        m_colorInput = null;

        updateValue( instance );
    }

    public override function updateValue(value : Dynamic) {
        var red = Math.round( Reflect.field( value, 'r' ) * 255 );
        var green = Math.round( Reflect.field( value, 'g' ) * 255 );
        var blue = Math.round( Reflect.field( value, 'b' ) * 255 );
        var alpha = untyped Reflect.field( value, 'a' ).toPrecision( 4 );

        m_colorPreview.style.background = 'rgba(${red}, ${green}, ${blue}, ${alpha})';

        m_instance = value;
    }

    public override function remove() {
        clearColorInput( );

        super.remove( );
    }

    private function onPreviewClick(e : js.html.MouseEvent) {
        if (m_colorInput != null)
            return;

        m_colorInput = new ColorInput( m_instance );

        m_colorInput.addEventListener( 'color-changed', onColorChanged );
        m_colorInput.addEventListener( 'closed', onColorClosed );

        js.Browser.document.body.appendChild( m_colorInput );

        m_colorInput.show( e.clientX, e.clientY );

        var handlers = m_owner.getFieldHandlers( );

        for (handler in handlers) {
            if (Std.is( handler, ColorFieldInspector) && handler != this) {
                var colorHandler : ColorFieldInspector = cast handler;

                colorHandler.clearColorInput( );
            }
        }
    }

    private function onColorChanged(e : js.html.CustomEvent) {
        notifyChanged( m_field, e.detail.color );
    }

    private function onColorClosed(e : js.html.CustomEvent) {
        m_colorInput = null;
    }

    private function clearColorInput() {
        if (m_colorInput != null) {
            js.Browser.document.body.removeChild( m_colorInput );

            m_colorInput = null;
        }
    }
}
