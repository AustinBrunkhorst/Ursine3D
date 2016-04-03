#pragma once

#include <Array.h>
#include <Vec2.h>

struct EditorPreferences
{
    ursine::Vec2 windowLocation;
    ursine::Vec2 windowSize;
    
    ursine::Array<std::string> recentProjects;

    std::string lastOpenWorld;
    bool fullScreen;

    EditorPreferences(void) = default;
} Meta(Enable);