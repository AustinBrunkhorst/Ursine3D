/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** EditorPreferences.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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