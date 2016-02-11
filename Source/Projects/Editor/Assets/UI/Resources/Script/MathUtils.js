/**
 * Clamps a value
 * @param value
 * @param min
 * @param max
 */
Math.clamp = function(value, min, max) {
    return Math.min( Math.max( value, min ), max );
};

/**
 * Sets the maximum number of decimal places (rounded)
 * for a floating point number
 * @param value
 * @param decimalPlaces
 * @returns {number}
 */
Math.toMaxPrecision = function(value, decimalPlaces) {
    var precision = Math.pow( 10, decimalPlaces );

    return Math.round( value * precision ) / precision;
};