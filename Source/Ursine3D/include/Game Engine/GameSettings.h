#pragma once

namespace ursine
{
    struct GameSettings
    {
        std::string title;
        bool fullScreen;
        std::string startingWorld;
    } Meta(Enable);
}