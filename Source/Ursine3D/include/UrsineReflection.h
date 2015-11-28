#pragma once

#include "Meta.h"

#include "UrsineReflectionExternal.h"

#include "UrsineTypes.h"

///////////////////////////////////////////////////////////////////////////////
// Math
///////////////////////////////////////////////////////////////////////////////

#include "Vec2.h"
#include "Vec3.h"
#include "SVec3.h"
#include "Vec4.h"
#include "SVec4.h"

///////////////////////////////////////////////////////////////////////////////
// Core Systems
///////////////////////////////////////////////////////////////////////////////

#include "Application.h"

#include "GfxAPI.h"
#include "Color.h"

/// UI

#include "UIManager.h"
#include "NativeJSClass.h"
#include "NativeJSFunction.h"

/// Window Management

#include "WindowManager.h"

/// Input Management

#include "GamepadManager.h"
#include "KeyboardManager.h"
#include "MouseManager.h"

/// Screen Management

#include "ScreenManager.h"

/// Audio

#include "AudioManager.h"

/// Utilities

#include "TimerManager.h"
#include "TweenManager.h"

/// Game Engine

#include "NativeComponents.h"
#include "NativeEntitySystems.h"

// define it last
#define URSINE_REFLECTION_INCLUDED