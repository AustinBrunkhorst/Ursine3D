#pragma once

////////////////////////////////////////////////////////////////////////////////
// Global Defines
////////////////////////////////////////////////////////////////////////////////

#define URSINE_ENGINE

// define this to show TODO items as warnings
// #define URSINE_TODO_AS_WARNINGS

#include "MacroUtils.h"

#include "UrsineLogTools.h"

// This is to stop windef.h conflicting with min and max
// http://stackoverflow.com/questions/14657474/conflict-of-windows-h-and-limits
#define NOMINMAX

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

#ifdef PLATFORM_WINDOWS
#define snprintf _snprintf
#endif

////////////////////////////////////////////////////////////////////////////////
// Meta
////////////////////////////////////////////////////////////////////////////////

#include "MetaManager.h"
#include "ReflectionDatabase.h"
#include "Meta.h"

////////////////////////////////////////////////////////////////////////////////
// Memory Tracking
////////////////////////////////////////////////////////////////////////////////

#include "UrsineMemory.h"

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

#include <time.h>
#include <stdio.h>
#include <math.h>

///////////////////////////////////////////////////////////////////////////////
// Boost
///////////////////////////////////////////////////////////////////////////////

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

///////////////////////////////////////////////////////////////////////////////
// GLEW/GLUT
///////////////////////////////////////////////////////////////////////////////

#include "GL/glew.h"

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

///////////////////////////////////////////////////////////////////////////////
// Misc Stuff
///////////////////////////////////////////////////////////////////////////////

#include "UrsineTypes.h"
#include "Utils.h"

#include "Color.h"

#include "Timer.h"
#include "Tween.h"
#include "TweenID.h"
#include "LocalTweenManager.h"

#include "FastDelegate.h"

#include "Screen.h"

#include "EventDispatcher.h"
#include "Application.h"

#include "Randomizer.h"
