package ursine.api.dom;

class ElementUtils {
    public static function once(target : js.html.Element, event : String, handler : Dynamic) : Void {
        var callback : Dynamic = null;

        callback = function(e) {
            target.addEventListener( event, callback );

            handler( e );
        };

        target.addEventListener( event, callback );
    }
}