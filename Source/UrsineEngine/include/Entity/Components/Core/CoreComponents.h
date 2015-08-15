/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CoreComponent.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

////////////////////////////////////////////////////////////////////////////////
// Include all core engine components here.
// IMPORTANT: RegisterComponent(), etc. must be be called inline with each
// component's definition (the header file).
////////////////////////////////////////////////////////////////////////////////

#ifdef URSINE_ENGINE
URSINE_ERROR("CoreComponents.h cannot be included in UrsineEngine")
#endif

#define REGISTER_COMPONENTS

// NOTE: Transform must be included first 
// (EntityManager assumes it has an ID of zero)
#include "TransformComponent.h"

#include "RenderableComponent.h"

#include "ParticleEmitterComponent.h"

#include "AnimationComponent.h"

#include "Rigidbody2DComponent.h"

#include "CreatorComponent.h"

#include "ZoneComponent.h"

#include "AudioEmittersComponent.h"

#include "KinematicChildComponent.h"

#include "OrbitDataComponent.h"

#undef REGISTER_COMPONENTS
