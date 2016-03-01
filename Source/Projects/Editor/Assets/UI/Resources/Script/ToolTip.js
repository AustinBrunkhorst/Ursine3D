var ToolTip = { };

// delay in milliseconds to delay showing
ToolTip.delay = 200;
ToolTip.fadeDuration = 125;

ToolTip._bound = { };
ToolTip._open = [ ];
ToolTip._nextID = 0;

ToolTip.bind = function(element, text) {
    var existing = ToolTip._bound[ element.__tid ];

    if (existing instanceof ToolTipItem) {
        existing.setText( text );
    } else {
        var id = element.__tid = ToolTip._nextID++;

        ToolTip._bound[ id ] = new ToolTipItem( element, text );
    }
};

ToolTip.unbind = function(element) {
    var existing = ToolTip._bound[ element.__tid ];

    if (existing instanceof ToolTipItem) {
        existing.unbind( );

        delete ToolTip._bound[ element.__tid ];
    }
};

ToolTip.unbindAll = function(element) {
    for (var id in ToolTip._bound) {
        var existing = ToolTip._bound[ id ];

        if (existing instanceof ToolTipItem) {
            existing.unbind( );
        }
    }

    ToolTip._bound = [ ];
};

function ToolTipItem(element, text) {
    this.element = element;
    this.text = text;

    this._enterHandler = this._onElementMouseEnter.bind( this );
    this._leaveHandler = this._onElementMouseLeave.bind( this );
    this._mouseMoveHandler = this._onElementMouseMove.bind( this );
    this._removeHandler = this._onElementRemoved.bind( this );

    this._enterTimeout = null;

    this._ttElement = null;

    this._mouseX = -9999;
    this._mouseY = -9999;

    this.element.addEventListener( 'mouseenter', this._enterHandler );
    this.element.addEventListener( 'mouseleave', this._leaveHandler );
    this.element.addEventListener( 'mousemove', this._mouseMoveHandler );
}

ToolTipItem.prototype.setText = function(text) {
    this.text = text;

    if (this._ttElement !== null) {
        this._ttElement.innerHTML = text;
    }
};

ToolTipItem.prototype.unbind = function() {
    this.element.removeEventListener( 'mouseenter', this._enterHandler );
    this.element.removeEventListener( 'mouseleave', this._leaveHandler );
    this.element.removeEventListener( 'mousemove', this._mouseMoveHandler );

    this._close( );
};

ToolTipItem.prototype._open = function() {
    // close all other tooltips
    for (var i = 0; i < ToolTip._open.length; ++i)
        ToolTip._open[ i ]._close( );

    ToolTip._open = [ ];

    this._ttElement = document.createElement( 'div' );

    this._ttElement.classList.add( 'tooltip' );

    this._ttElement.innerHTML = this.text;

    document.querySelector( '#tooltip-container' ).appendChild( this._ttElement );

    var ttElement = this._ttElement;

    // race condition
    setTimeout(function() {
        ttElement.classList.add( 'opened' );
    }, 0 );

    this._updatePosition( );

    document.body.addEventListener( 'DOMNodeRemoved', this._removeHandler, true );

    ToolTip._open.push( this );
};

ToolTipItem.prototype._close = function() {
    if (this._ttElement !== null) {
        this._ttElement.classList.remove( 'opened' );

        var ttElement = this._ttElement;

        setTimeout( function() {
            if (ttElement.parentNode)
                ttElement.parentNode.removeChild( ttElement );
        }, ToolTip.fadeDuration );

        this._ttElement = null;
    }

    document.body.removeEventListener( 'DOMNodeRemoved', this._removeHandler, true );
};

ToolTipItem.prototype._updatePosition = function() {
    var bounds = this._ttElement.getBoundingClientRect( );

    var x = Math.min( this._mouseX, window.innerWidth - bounds.width );

    // y is offset by the height to align it above the cursor
    var y = Math.min( this._mouseY - bounds.height, window.innerHeight - bounds.height );

    this._ttElement.style.left = x +'px';
    this._ttElement.style.top = y +'px';
};

ToolTipItem.prototype._onElementMouseEnter = function(e) {
    // already open
    if (this._ttElement !== null)
        return;

    this._mouseX = e.clientX;
    this._mouseY = e.clientY;

    this._enterTimeout = setTimeout( this._open.bind( this ), ToolTip.delay );

    e.stopPropagation( );
    e.stopImmediatePropagation( );
};

ToolTipItem.prototype._onElementMouseLeave = function(e) {
    clearTimeout( this._enterTimeout );

    this._close( );

    e.stopPropagation( );
    e.stopImmediatePropagation( );
};

ToolTipItem.prototype._onElementMouseMove = function(e) {
    this._mouseX = e.clientX;
    this._mouseY = e.clientY;

    if (this._ttElement !== null)
        this._updatePosition( );
};

ToolTipItem.prototype._onElementRemoved = function(e) {
    if (e.target === this.element || e.target.contains( this.element ))
        ToolTip.unbind( this.element );
};