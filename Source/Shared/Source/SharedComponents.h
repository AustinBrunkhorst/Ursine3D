﻿/* ----------------------------------------------------------------------------
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
#include "PlayerLogic/PlayerSpawnPointComponent.h"
#include "PlayerLogic/CharacterController/CharacterControllerComponent.h"

/////////////////////////////////
//        Damage System        //
/////////////////////////////////

#include "DamageSystem/ExplosionComponent.h"
#include "DamageSystem/CritSpotComponent.h"
#include "DamageSystem/DamageOnCollideComponent.h"
#include "DamageSystem/DamageTextComponent.h"

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
//        Trails System        //
/////////////////////////////////

#include "Trails/TrailComponent.h"

/////////////////////////////////
//            AI               //
/////////////////////////////////

#include "AI/WaypointComponent.h"
#include "AI/WaypointAgentComponent.h"
#include "AI/AIMovementControllerComponent.h"
#include "AI/AIHordeReasoner.h"

#include "EnemyDeathComponent.h"

#include "BossAIComponent.h"
#include "VineAIComponent.h"

/////////////////////////////////
//       Level Logic           //
/////////////////////////////////

#include "LevelSegmentManager/LevelSegmentManagerComponent.h"

#include "LevelSegmentManager/SegmentResources/UnloadResourceComponent.h"
#include "LevelSegmentManager/SegmentResources/TutorialResourcesComponent.h"
#include "LevelSegmentManager/SegmentResources/CombatBowl1ResourcesComponent.h"
#include "LevelSegmentManager/SegmentResources/BossRoomResourcesComponent.h"
#include "LevelSegmentManager/SegmentLogic/Misc/SegmentChangeTriggerComponent.h"

/////////////////////////////////
//       Entity Animation      //
/////////////////////////////////

#include "EntityAnimation/EntityAnimatorComponent.h"
#include "EntityAnimation/EntityAnimatorGroupComponent.h"
#include "EntityAnimation/EntityAnimatorFocusPointComponent.h"
#include "EntityAnimation/EntityAnimatorFocusModifierComponent.h"

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

/////////////////////////////////
//             FX              //
/////////////////////////////////

#include "FX/AnimateParticleResetComponent.h"

/////////////////////////////////
//             Misc            //
/////////////////////////////////

#include "Misc/AnimateTutorialTextComponent.h"
#include "LevelSegmentManager/SegmentLogic/Misc/AnimateOnSegmentComponent.h"

/////////////////////////////////
//           Physics           //
/////////////////////////////////

#include "Physics/ApplyImpulseOnHitComponent.h"
