/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SharedComponents.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "Components/CharacterFireControllerComponent.h"
#include "Components/CharacterControllerComponent.h"
#include "Components/PlayerInputComponent.h"
#include "Components/HealthComponent.h"
#include "Components/SpawnpointComponent.h"
#include "Components/CommandQueueComponent.h"
#include "Components/CommandInputControllerComponent.h"
#include "Components/TeamComponent.h"
#include "Components/RecorderComponent.h"
#include "Components/RandomSlerpComponent.h"
#include "Components/PlayerAnimationComponent.h"
#include "Components/TimeDeathComponent.h"
#include "Components/ProjectileComponent.h"
#include "Components/DamageOnCollideComponent.h"
#include "Components/PlayerIdComponent.h"
#include "Components/ControlPointComponent.h"
#include "Components/WallComponent.h"
#include "Components/CritSpotComponent.h"
#include "Components/ForceComponent.h"
#include "Components/ExplosionComponent.h"


///////////////////////////////////////////////////////////////////////////////
//// Interacton Components 
///////////////////////////////////////////////////////////////////////////////

#include "Components/Interactables/InteractionBay.h"
#include "Components/Interactables/InteractableComponent.h"
#include "Components/Interactables/Pickups/WeaponPickup.h"


///////////////////////////////////////////////////////////////////////////////
//// WeaponLogic Components 
///////////////////////////////////////////////////////////////////////////////

// weapons
#include "Components/WeaponLogic/Weapons/BaseWeaponComponent.h"
#include "Components/WeaponLogic/Weapons/HitscanWeaponComponent.h"

// micelaneous weapon logic
#include "Components/WeaponLogic/AmmoPickUpComponent.h"


/////////////////////////////////
////          AI             ////
/////////////////////////////////

#include "Components/AI/WaypointComponent.h"
#include "Components/AI/WaypointAgentComponent.h"
#include "Components/AI/AIMovementControllerComponent.h"
#include "Components/AI/AIHordeReasoner.h"

/////////////////////////////////
////     Level Logic         ////
/////////////////////////////////

#include "Components/LevelLogic/ElevatorLiftMoverComponent.h"



