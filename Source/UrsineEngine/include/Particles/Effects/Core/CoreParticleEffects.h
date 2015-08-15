/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CoreParticleEffects.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

////////////////////////////////////////////////////////////////////////////////
// Include all core engine particle effects here.
// IMPORTANT: RegisterParticleEffect(), etc. must be be called inline with each
// component's definition (the header file).
////////////////////////////////////////////////////////////////////////////////

#ifdef URSINE_ENGINE
URSINE_ERROR("CoreParticleEffects.h cannot be included in UrsineEngine")
#endif

#define REGISTER_PARTICLE_EFFECT

#include "ColorSetterEffect.h"
#include "LifeSpanEffect.h"
#include "MoveToCircleEffect.h"
#include "MoveToColorEffect.h"
#include "MoveToPositionEffect.h"
#include "MoveToScaleEffect.h"
#include "OrbitEffect.h"
#include "PositionSetterEffect.h"
#include "ScaleSetterEffect.h"
#include "TextureSetterEffect.h"

#undef REGISTER_PARTICLE_EFFECT
