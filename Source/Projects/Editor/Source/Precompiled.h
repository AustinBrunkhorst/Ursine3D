/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Precompiled.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#define URSINE_OUTPUT_WARNINGS 1

#include <windows.h>

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <functional>

#include <MacroUtils.h>
#include <UrsineLogTools.h>

////////////////////////////////////////////////////////////////////////////////
// Global Engine Stuff
////////////////////////////////////////////////////////////////////////////////

#include <UrsineBuildConfig.h>
#include <UrsinePlatform.h>

////////////////////////////////////////////////////////////////////////////////
// SDL
////////////////////////////////////////////////////////////////////////////////

#include <SDL.h>

////////////////////////////////////////////////////////////////////////////////
// Core Engine Stuff
////////////////////////////////////////////////////////////////////////////////

#include <EventDispatcher.h>

#include <UrsineLogTools.h>

#include <UrsineMath.h>

#include <TimeSpan.h>

#include <TweenID.h>
#include <Tween.h>

////////////////////////////////////////////////////////////////////////////////
// Game Engine Stuff
////////////////////////////////////////////////////////////////////////////////

#include <EntityManager.h>

#include "EditorEntityManager.h"
#include "Project.h"