/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** UrsinePrecompiled.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

////////////////////////////////////////////////////////////////////////////////
// Global Defines
////////////////////////////////////////////////////////////////////////////////

#define URSINE_ENGINE

// define this to show TODO items as warnings
// #define URSINE_TODO_AS_WARNINGS

#include "MacroUtils.h"

#include "UrsineLogTools.h"

////////////////////////////////////////////////////////////////////////////////
// Configuration Detection
////////////////////////////////////////////////////////////////////////////////

#include "UrsineBuildConfig.h"

////////////////////////////////////////////////////////////////////////////////
// Platform Detection
////////////////////////////////////////////////////////////////////////////////

#include "UrsinePlatform.h"

////////////////////////////////////////////////////////////////////////////////
// Platform Specific Definitions
////////////////////////////////////////////////////////////////////////////////

#if defined(PLATFORM_WINDOWS)

#define snprintf _snprintf

#define _WIN32_WINNT 0x0600

#endif

///////////////////////////////////////////////////////////////////////////////
// Standard Libraries
///////////////////////////////////////////////////////////////////////////////

#include <exception>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <typeinfo>
#include <random>

#include <time.h>
#include <stdio.h>
#include <math.h>

///////////////////////////////////////////////////////////////////////////////
// Containers
///////////////////////////////////////////////////////////////////////////////

#include "Array.h"
#include "BinaryData.h"

////////////////////////////////////////////////////////////////////////////////
// Meta
////////////////////////////////////////////////////////////////////////////////

#include "ArrayWrapper.h"
#include "ArrayWrapperBase.h"
#include "ArrayWrapperContainer.h"

#include "ArrayVariantContainer.h"

#include "MetaManager.h"
#include "ReflectionDatabase.h"
#include "Meta.h"

////////////////////////////////////////////////////////////////////////////////
// Memory Tracking
////////////////////////////////////////////////////////////////////////////////

#include "UrsineMemory.h"

///////////////////////////////////////////////////////////////////////////////
// Boost
///////////////////////////////////////////////////////////////////////////////

#include "FileSystem.h"

///////////////////////////////////////////////////////////////////////////////
// Chromium Embedded Framework
///////////////////////////////////////////////////////////////////////////////

#include "cef_app.h"
#include "cef_browser.h"

///////////////////////////////////////////////////////////////////////////////
// SDL2
///////////////////////////////////////////////////////////////////////////////

#include "SDL.h"

////////////////////////////////////////////////////////////////////////////////
// Serialization
////////////////////////////////////////////////////////////////////////////////

#include "JsonSerializer.h"

///////////////////////////////////////////////////////////////////////////////
// Math Components
///////////////////////////////////////////////////////////////////////////////

#include "UrsineMath.h"

#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Mat3.h"
#include "Mat4.h"

#include "SIMD.h"
#include "SVec3.h"
#include "SVec4.h"
#include "SMat3.h"
#include "SMat4.h"
#include "SQuat.h"

#include "Curves.h"

///////////////////////////////////////////////////////////////////////////////
// Resource Management
///////////////////////////////////////////////////////////////////////////////

#include "ResourcePipelineConfig.h"

///////////////////////////////////////////////////////////////////////////////
// Misc Stuff
///////////////////////////////////////////////////////////////////////////////

#include "Scene.h"

#include "UrsineTypes.h"
#include "Utils.h"

#include "Color.h"

#include "Timer.h"
#include "Tween.h"
#include "TweenID.h"
#include "LocalTweenManager.h"

#include "FastDelegate.h"

#include "EventDispatcher.h"
#include "Application.h"

#include "Randomizer.h"

#include "EntityManager.h"
