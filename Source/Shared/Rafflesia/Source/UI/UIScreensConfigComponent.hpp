template<typename EventType>
void UIScreensConfig::triggerEvent(
    const char *screenName,
    const ursine::resources::ResourceReference &target,
    const std::string &eventName,
    const EventType &data
) const
{
    auto *screen = getScreen( target );

#if defined(URSINE_WITH_EDITOR)

    if (!screen)
    {
        UWarning( "Messaging screen '%s' that is invalid or not added.", screenName );

        return;
    }

#else

    UAssert( screen, "Messaging screen '%s' that is invalid or not added.", screenName );

#endif

    screen->TriggerEvent( eventName, data );
}