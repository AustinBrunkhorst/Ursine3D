/**
 * Clamps a value
 * @param value
 * @param min
 * @param max
 */
Math.clamp = function(value, min, max) {
    return Math.min( Math.max( value, min ), max );
};