var ElementUtils = { };

ElementUtils.once = function(element, event, handler) {
    var callback = function(e) {
        element.addEventListener( event, callback );

        handler( e );
    };

    element.addEventListener( event, callback );
};