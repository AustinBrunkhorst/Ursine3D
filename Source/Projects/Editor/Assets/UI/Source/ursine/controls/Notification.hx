package ursine.controls;

import js.html.HtmlElement;

@:enum
abstract NotificationType(Int) {
    inline var None = 0;
    inline var Warning = 1;
    inline var Error = 2;
    inline var Info = 3;
    inline var Question = 4;
}

@:native( "NotificationControl" )
extern class Notification extends PolymerElement {
    var dismissible : Bool;
    var type : NotificationType;
    var header : String;
    var message : String;

    // buttons container
    var buttons : HtmlElement;

    function new(type : NotificationType, message : String, ?header : String) : Void;

    function show(?duration : Int) : Void;
    function close() : Void;
}
