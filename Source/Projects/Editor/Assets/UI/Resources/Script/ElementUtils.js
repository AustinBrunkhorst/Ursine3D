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

ElementUtils.parent = function(element, selector) {
    var node = element.parentNode;

    for (; node && node !== document; node = node.parentNode) {
        if (node.matches( selector ))
            return node;
    }

    // couldn't find it
    return null;
};