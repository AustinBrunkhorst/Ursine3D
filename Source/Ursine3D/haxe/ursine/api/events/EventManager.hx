package ursine.api.events;

typedef EventHandler = Dynamic;

class EventManager {
    private var m_events : Map<String, Array<EventHandler>> = new Map<String, Array<EventHandler>>( );

    public function new() { }

    public function on(event : String, handler : EventHandler) : EventManager {
        var handlers = m_events.get( event );

        if (handlers == null) {
            handlers = new Array<EventHandler>( );

            m_events.set( event, handlers );
        }

        handlers.push( handler );

        return this;
    }

    public function off(event : String, handler : EventHandler) : EventManager {
        var handlers = m_events.get( event );

        if (handlers != null)
            handlers.remove( handler );

        return this;
    }

    public function one(event : String, handler : EventHandler) : EventManager {
        var oneShot : EventHandler = null;

        oneShot = function(e) {
            off( event, oneShot );

            return handler( e );
        };

        on( event, oneShot );

        return this;
    }

    public function trigger(event : String, data : Dynamic) : Bool {
        var handlers = m_events.get( event );

        // nothing to do here
        if (handlers == null)
            return true;

        var result = true;

        for (handler in handlers) {
            if (!handler( data ))
                result = false;
        }

        return result;
    }
}
