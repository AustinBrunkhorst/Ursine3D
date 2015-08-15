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

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#include "Matrix3.h"
#include "Matrix4.h"

///////////////////////////////////////////////////////////////////////////////
// Physics Components
///////////////////////////////////////////////////////////////////////////////

#include "AABB.h"
#include "BasePolygon.h"
#include "Collider.h"
#include "CollisionEventArgs.h"
#include "Collision.h"
#include "Ellipse.h"
#include "Manifold.h"
#include "PhysicsDebugDrawSystem.h"
#include "Polygon.h"
#include "RayCast.h"
#include "Rigidbody2DComponent.h"
#include "Shape.h"
#include "Simplex.h"
#include "SimulationConfig.h"
#include "SimulationSystem.h"
#include "DistanceJoint.h"
#include "LayerID.h"
#include "LayerMask.h"
#include "KinematicChildComponent.h"

///////////////////////////////////////////////////////////////////////////////
// Audio Components
///////////////////////////////////////////////////////////////////////////////

#include "AudioEmittersComponent.h"

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
#include "System.h"

#include "Randomizer.h"
