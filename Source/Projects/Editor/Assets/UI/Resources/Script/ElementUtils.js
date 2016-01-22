var ElementUtils = { };

ElementUtils.once = function(element, event, handler) {
    var callback = function(e) {
        element.addEventListener( event, callback );

        handler( e );
    };

    element.addEventListener( event, callback );
};

ElementUtils.childIndex = function(element) {
    var i = 0;

    while ((element = element.previousElementSibling) != null)
        ++i;

    return i;
};