#pragma once

#define UI_EVENT(name, definition)                 \
    struct name {                                  \
        static constexpr auto DefaultName = #name; \
        ##definition##                             \
    } Meta(Enable)                                 \

namespace ui_event
{
    namespace global
    {
        const auto GameplayStarted = "GameplayStarted";
    }

    UI_EVENT( ToggleHUD,
        bool toggled;
    );

    UI_EVENT( PlayerHealthUpdated,
        int playerID;
        float percent;
    );
}