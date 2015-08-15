#pragma once

////////////////////////////////////////////////////////////////////////////////
// Include all core engine component systems here.
// IMPORTANT: RegisterEntitySystem(), etc. must be be called inline with each
// systems's definition (the header file).
////////////////////////////////////////////////////////////////////////////////

#ifdef URSINE_ENGINE
URSINE_ERROR("CoreSystems.h cannot be included in UrsineEngine")
#endif

#define REGISTER_SYSTEMS

#include "RenderSystem.h"

#include "AnimationSystem.h"

#include "SimulationSystem.h"

#include "PhysicsDebugDrawSystem.h"

#include "MacroUtils.h"

#include "KinematicChildSystem.h"

#include "ParticleEmitterSystem.h"

#undef REGISTER_SYSTEMS