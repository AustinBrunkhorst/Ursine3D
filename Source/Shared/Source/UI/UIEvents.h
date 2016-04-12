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
        const auto GameStart = "GameplayStart";
        const auto GamePause = "GameplayPause";
        const auto GameResume = "GameplayResume";
        const auto GameExit = "GameplayExit";
        const auto GameRestart = "GameRestart";
    }

    UI_EVENT( ToggleHUD,
        bool toggled;
    );

    UI_EVENT( PlayerHealthUpdated,
        int playerID;
        float percent;
    );

    UI_EVENT( PlayerAmmoUpdated,
        int playerID;
        float percent;
    );
}