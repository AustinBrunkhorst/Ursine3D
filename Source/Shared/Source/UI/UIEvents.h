#pragma once

#include "LevelSegments.h"

#include <Color.h>

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
        std::string enemyName;
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

    UI_EVENT( PlayerReviveToggle,
        // True means revive trigger has been entered,
        // false means revive trigger has been exited
        bool toggle;
        int playerReviving;
        int playerDowned;
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

    UI_EVENT( TutorialDialogue,
        ursine::Array<std::string> dialogueLines;
        float totalTimeForDialogue;
    );

    UI_EVENT( ClearDialogue,
        ;
    );

    UI_EVENT( ScreenFadeEffect,
        // Time it takes to fade into the color
        float fadeInTime;
        // Time we're paused at the color
        float pauseTime;
        // Time it takes to fade from the color to gameplay
        float fadeOutTime;

        ursine::Color screenColor;
    );
}