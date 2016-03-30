namespace ursine
{
    template<typename EventType>
    inline void UIScreen::TriggerEvent(const EventType &data) const
    {
        TriggerEvent( EventType::DefaultName, data );
    }

    template<typename EventType>
    inline void UIScreen::TriggerEvent(const std::string &eventName, EventType &data) const
    {
        static const auto eventType = typeof( EventType );

        UAssert( 
            eventType.IsValid( ), 
            "Unknown event type data '%s'. Is it registered with reflection?",
            typeid( EventType ).name( )
        );

        m_manager->MessageScreenRemote( this, eventName, meta::Variant( data ).SerializeJson( ) );
    }
}