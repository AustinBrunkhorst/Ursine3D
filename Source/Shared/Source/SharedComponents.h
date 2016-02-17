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

/////////////////////////////////
//        Health System        //
/////////////////////////////////

#include "HealthSystem/HealthComponent.h"
#include "HealthSystem/WallComponent.h"

/////////////////////////////////
//        Miscelaneous         //
/////////////////////////////////

#include "Misc/ForceComponent.h"
#include "Misc/TimeDeathComponent.h"

/////////////////////////////////
//        Player Logic         //
/////////////////////////////////

#include "PlayerLogic/PlayerIdComponent.h"
#include "PlayerLogic/CharacterController/CharacterControllerComponent.h"

/////////////////////////////////
//        Damage System        //
/////////////////////////////////

#include "DamageSystem/ExplosionComponent.h"
#include "DamageSystem/CritSpotComponent.h"
#include "DamageSystem/DamageOnCollideComponent.h"

/////////////////////////////////
//    Interaction System       //
/////////////////////////////////

#include "InteractionSystem/InteractionBayComponent.h"
#include "InteractionSystem/InteractableComponent.h"
#include "InteractionSystem/Pickups/WeaponPickup.h"
#include "InteractionSystem/Triggers/ControlPointComponent.h"

/////////////////////////////////
//        Weapon System        //
/////////////////////////////////

#include "WeaponSystem/Weapons/BaseWeaponComponent.h"
#include "WeaponSystem/Weapons/HitscanWeaponComponent.h"
#include "WeaponSystem/AmmoPickUpComponent.h"
#include "WeaponSystem/Weapons/FirePosComponent.h"
#include "WeaponSystem/Inventory/InventoryComponent.h"
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

#include "LevelManager/Logic/ElevatorLiftMoverComponent.h"

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

/////////////////////////////////
//       Command System        //
/////////////////////////////////

#include "CommandSystem/InputControllerComponent.h"
#include "CommandSystem/CommandQueueComponent.h"
