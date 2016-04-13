#pragma once

#include "LevelSegments.h"

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
        int playerID;
    );

    UI_EVENT( PlayerHealthUpdated,
        int playerID;
        float percent;
    );

    UI_EVENT( PlayerDamaged,
        int playerID;
        float damage;
        bool front;
        bool back;
        bool right;
        bool left;
    );

    UI_EVENT( HealthTrackStart,
        int playerID;
        std::string NameOfEnemy;
        float healthPercent;
    );

    UI_EVENT( HealthTrackUpdate,
        int playerID;
        float healthPercent;
    );

    UI_EVENT( HealthTrackEnd,
        int playerID;
        bool enemyKilled;
    );

    UI_EVENT( PlayerAmmoUpdated,
        int playerID;
        float percent;
    );

    UI_EVENT( BothPlayersDied,
        LevelSegments currentSegment;
    );

    UI_EVENT( PlayerReviveUpdate,
        int playerReviving;
        int playerDowned;
        float percent;
    );

    UI_EVENT( PlayerReviveSuccess,
        int playerReviving;
        int playerRevived;
    );
}