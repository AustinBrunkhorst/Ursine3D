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

#include "Components/PlayerInputComponent.h"
#include "Components/HealthComponent.h"
#include "Components/CommandQueueComponent.h"
#include "Components/CommandInputControllerComponent.h"
#include "Components/TeamComponent.h"
#include "Components/TimeDeathComponent.h"
#include "Components/DamageOnCollideComponent.h"
#include "Components/PlayerIdComponent.h"
#include "Components/ControlPointComponent.h"
#include "Components/WallComponent.h"
#include "Components/CritSpotComponent.h"
#include "Components/ForceComponent.h"
#include "Components/ExplosionComponent.h"

/////////////////////////////////
//    Character Controller     //
/////////////////////////////////

#include "CharacterController/CharacterControllerComponent.h"

/////////////////////////////////
//    Interaction System       //
/////////////////////////////////

#include "InteractionSystem/InteractionBayComponent.h"
#include "InteractionSystem/InteractableComponent.h"
#include "InteractionSystem/Pickups/WeaponPickup.h"

/////////////////////////////////
//        Weapon System        //
/////////////////////////////////

#include "WeaponSystem/Weapons/BaseWeaponComponent.h"
#include "WeaponSystem/Weapons/HitscanWeaponComponent.h"
#include "WeaponSystem/AmmoPickUpComponent.h"
#include "WeaponSystem/FirePosComponent.h"
#include "WeaponSystem/InventoryComponent.h"
#include "WeaponSystem/Projectiles/ProjectileComponent.h"

/////////////////////////////////
//            AI               //
/////////////////////////////////

#include "AI/WaypointComponent.h"
#include "AI/WaypointAgentComponent.h"
#include "AI/AIMovementControllerComponent.h"
#include "AI/AIHordeReasoner.h"

/////////////////////////////////
//       Level Logic           //
/////////////////////////////////

#include "LevelEvents/Logic/ElevatorLiftMoverComponent.h"

/////////////////////////////////
//       Camera Animation      //
/////////////////////////////////

#include "CameraAnimation/CameraAnimatorComponent.h"
#include "CameraAnimation/CameraAnimatorNodeComponent.h"
#include "CameraAnimation/CameraAnimatorFocusPointComponent.h"

/////////////////////////////////
//       Spawning System       //
/////////////////////////////////

#include "SpawnerSystem/SpawnerGroupComponent.h"
#include "SpawnerSystem/SpawnerComponent.h"
#include "SpawnerSystem/SpawnPatternContainerComponent.h"
