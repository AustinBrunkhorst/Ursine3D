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
        var time = createTimeString( );

        // @@@TODO: remove when UI level console implemented
        trace( '${time}: ${info}' );

        instance.m_logContainer.appendChild(
            instance.createLogElement( time, info )
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

    private function createLogElement(timeText : String, infoText : String) {
        var element = js.Browser.document.createDivElement( );

        var time = js.Browser.document.createSpanElement( );
        {
            time.innerText = timeText;

            element.appendChild( time );
        }

        var contents = js.Browser.document.createSpanElement( );
        {
            contents.innerHTML = infoText;

            element.appendChild( contents );
        }

        return element;
    }

    private static function createTimeString() : String {
        var date = Date.now( );

        var hours = date.getHours( );

        var h = (hours % 12) + 1;
        var m = StringTools.lpad( cast date.getMinutes( ), '0', 2 );
        var s = StringTools.lpad( cast date.getSeconds( ), '0', 2 );
        var z = (hours < 12) ? 'AM' : 'PM';

        return '${h}:${m}:${s} ${z}';
    }

    private function onStatusBarClick(e : js.html.MouseEvent) {

    }
}
