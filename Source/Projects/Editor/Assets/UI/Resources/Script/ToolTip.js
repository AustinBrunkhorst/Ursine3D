var ToolTip = { };

// delay in milliseconds to delay showing
ToolTip.delay = 300;

ToolTip._bound = { };
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

function ToolTipItem(element, text) {
    this.element = element;
    this.text = text;

    this._enterHandler = this._onElementMouseEnter.bind( this );
    this._leaveHandler = this._onElementMouseLeave.bind( this );
    this._mouseMoveHandler = this._onElementMouseMove.bind( this );

    this._enterTimeout = null;

    this._ttElement = null;

    this._mouseX = -9999;
    this._mouseY = -9999;

    this.element.addEventListener( 'mouseover', this._enterHandler, true );
    this.element.addEventListener( 'mouseout', this._leaveHandler, true );
    this.element.addEventListener( 'mousemove', this._mouseMoveHandler, true );
}

ToolTipItem.prototype.setText = function(text) {
    this.text = text;

    if (this._ttElement !== null) {
        this._ttElement.innerHTML = text;
    }
};

ToolTipItem.prototype.unbind = function() {
    this.element.removeEventListener( 'mouseover', this._enterHandler, true );
    this.element.removeEventListener( 'mouseout', this._leaveHandler, true );
    this.element.removeEventListener( 'mousemove', this._mouseMoveHandler, true );

    this._close( );
};

ToolTipItem.prototype._open = function() {
    this._ttElement = document.createElement( 'div' );

    this._ttElement.classList.add( 'tooltip' );

    this._ttElement.innerHTML = this.text;

    document.querySelector( '#tooltip-container' ).appendChild( this._ttElement );

    this._updatePosition( );
};

ToolTipItem.prototype._close = function() {
    if (this._ttElement !== null) {
        this._ttElement.parentNode.removeChild( this._ttElement );

        this._ttElement = null;
    }
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
    this._mouseX = e.clientX;
    this._mouseY = e.clientY;

    this._enterTimeout = setTimeout( this._open.bind( this ), ToolTip.delay );

    e.preventDefault( );
    e.stopPropagation( );
    e.stopImmediatePropagation( );
};

ToolTipItem.prototype._onElementMouseLeave = function(e) {
    clearTimeout( this._enterTimeout );

    this._close( );

    e.preventDefault( );
    e.stopPropagation( );
    e.stopImmediatePropagation( );
};

ToolTipItem.prototype._onElementMouseMove = function(e) {
    this._mouseX = e.clientX;
    this._mouseY = e.clientY;

    if (this._ttElement !== null) {
        this._updatePosition( );
    }
};