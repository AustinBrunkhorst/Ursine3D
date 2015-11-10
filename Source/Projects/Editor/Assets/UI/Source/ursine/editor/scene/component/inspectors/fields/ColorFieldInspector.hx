package ursine.editor.scene.component.inspectors.fields;

import ursine.controls.ColorInput;
import ursine.editor.scene.component.ComponentDatabase;

@fieldInspector( "ursine::Color" )
class ColorFieldInspector extends FieldInspectionHandler {
    var m_colorPreview : js.html.DivElement;
    var m_colorInput : ColorInput;

    public function new(owner : ComponentInspectionHandler, instance : Dynamic, field : NativeField, type : NativeType) {
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
        var red = Reflect.field( value, 'r' ) * 255;
        var green = Reflect.field( value, 'g' ) * 255;
        var blue = Reflect.field( value, 'b' ) * 255;
        var alpha = Reflect.field( value, 'a' ) * 255;

        m_colorPreview.style.background = 'rgba(${red}, ${green}, ${blue}, ${alpha})';
    }

    private function onPreviewClick(e : js.html.MouseEvent) {
        if (m_colorInput != null)
            return;

        m_colorInput = new ColorInput( m_instance );

        m_colorInput.style.left = '${e.clientX}px';
        m_colorInput.style.top = '${e.clientY}px';

        js.Browser.document.body.appendChild( m_colorInput );
    }
}
