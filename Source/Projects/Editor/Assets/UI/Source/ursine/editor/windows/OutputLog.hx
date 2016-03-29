package ursine.editor.windows;

class OutputLog extends WindowHandler {
    public static var instance : OutputLog;

    private var m_isOpen : Bool;

    private var m_logContainer : js.html.DivElement;

    public function new() {
        instance = this;

        super( );

        m_isOpen = false;

        initElements( );
    }

    public static function log(info : String) {
        instance.m_logContainer.appendChild(
            instance.createLogElement( info )
        );

        var lastLine = info.split( '\n' )[ 0 ];

        Editor.instance.setStatusText( lastLine );
    }

    private function initElements() {
        m_logContainer = js.Browser.document.createDivElement( );
        {
            window.container.appendChild( m_logContainer );
        }

        js.Browser.document.querySelector( '#status-bar' )
            .addEventListener( 'click', onStatusBarClick );
    }

    private function createLogElement(info : String) {
        var element = js.Browser.document.createDivElement( );

        var time = js.Browser.document.createSpanElement( );
        {
            var date = Date.now( );

            var hours = date.getHours( );

            var h = (hours % 12) + 1;
            var m = StringTools.lpad( cast date.getMinutes( ), '0', 2 );
            var s = StringTools.lpad( cast date.getSeconds( ), '0', 2 );
            var z = (hours < 12) ? 'AM' : 'PM';

            time.innerText = '${h}:${m}:${s} ${z}';

            element.appendChild( time );
        }

        var contents = js.Browser.document.createSpanElement( );
        {
            contents.innerHTML = info;

            element.appendChild( contents );
        }

        return element;
    }

    private function onStatusBarClick(e : js.html.MouseEvent) {

    }
}
