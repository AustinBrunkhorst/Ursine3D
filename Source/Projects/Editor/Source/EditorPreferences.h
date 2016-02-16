#pragma once

#include <Array.h>
#include <Vec2.h>

struct EditorPreferences
{
    bool windowMaximized;
    ursine::Vec2 windowLocation;
    ursine::Vec2 windowSize;
    
    ursine::Array<std::string> recentProjects;

    EditorPreferences(void) = default;
} Meta(Enable);