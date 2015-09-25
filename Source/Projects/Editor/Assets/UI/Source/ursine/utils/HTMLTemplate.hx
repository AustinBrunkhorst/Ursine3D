package ursine.utils;

import js.html.DOMElement;
class HTMLTemplate {
    private static var m_cache : Map<String, HTMLTemplate> = new Map<String, HTMLTemplate>( );

    private var m_template : haxe.Template;

    public static function load(name : String) : HTMLTemplate {
        var cached = m_cache.get( name );

        if (cached == null) {
            var template = js.Browser.document.querySelector( 'link[rel="import"][href="${name}"]' );

            if (template == null)
                throw 'Unable to load template ${name}';

            // haxe doesn't like "import"
            var data = new HTMLTemplate( untyped __js__( "template.import.body.innerHTML" ) );

            m_cache.set( name, data );

            return data;
        }

        return cached;
    }

    public function compile(?context : Dynamic, ?macros : Dynamic) : js.html.DOMElement {
        var div = js.Browser.document.createDivElement( );

        div.innerHTML = m_template.execute( context ? context : { }, macros );

        return cast div.childNodes[ 0 ];
    }

    private function new(data : String) {
        m_template = new haxe.Template( data );
    }
}
