package retrospect.screens;

import ursine.utils.EventManager;
import ursine.utils.IEventContainer;

class BasicMenu implements IEventContainer {
    public var events : EventManager;
    public var container(get, null) : js.html.DivElement;
    public var activeItem(get, null) : js.html.DivElement;

    private var m_container : js.html.DivElement;
    private var m_activeItem : js.html.DivElement;

    public function new(container : js.html.DivElement) {
        events = new EventManager( );

        m_container = container;

        m_activeItem = cast container.querySelector( '.active' );
    }

    public function up() {
        var previous = findPrevious( );

        if (previous != null) {
            setActive( previous );

            events.trigger( 'up', {
                item: previous
            } );
        }
    }

    public function down() {
        var next = findNext( );

        if (next != null) {
            setActive( next );

            events.trigger( 'down', {
                item: next
            } );
        }
    }

    private function get_container() : js.html.DivElement {
        return m_container;
    }

    private function get_activeItem() : js.html.DivElement {
        return m_activeItem;
    }

    private function setActive(item : js.html.DivElement) {
        if (m_activeItem == item)
            return;

        m_activeItem.classList.remove( 'active' );
        item.classList.add( 'active' );

        m_activeItem = item;
    }

    private function findPrevious() : js.html.DivElement {
        var previous : js.html.Element = m_activeItem;

        while ((previous = previous.previousElementSibling) != null) {
            if (!previous.classList.contains( 'no-select' ))
                return cast previous;
        }

        return null;
    }

    private function findNext() : js.html.DivElement {
        var next : js.html.Element = m_activeItem;

        while ((next = next.nextElementSibling) != null) {
            if (!next.classList.contains( 'no-select' ))
                return cast next;
        }

        return null;
    }
}
