var ObjectUtils = { };

ObjectUtils.clone = function(obj) {
    if (null == obj || "object" != typeof obj)
        return obj;

    var copy = obj.constructor( );

    for (var attr in obj) {
        if (obj.hasOwnProperty( attr ))
            copy[ attr ] = obj[ attr ];
    }

    return copy;
};

ObjectUtils.lastElement = function(arrayLike) {
    if (!arrayLike.length)
        return null;

    return arrayLike[ arrayLike.length - 1 ];
};