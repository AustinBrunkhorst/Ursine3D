#pragma once

namespace ursine
{
    struct GameSettings
    {
        std::string windowTitle;
        bool windowFullScreen;
        bool windowResizable;
        std::string startingWorld;

        GameSettings(void) = default;
    } Meta(Enable);
}