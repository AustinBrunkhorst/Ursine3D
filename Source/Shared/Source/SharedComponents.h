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
#include "HealthSystem/HealthRegenComponent.h"

/////////////////////////////////
//        Miscelaneous         //
/////////////////////////////////

#include "Misc/ForceComponent.h"
#include "Misc/TimeDeathComponent.h"
#include "Misc/BuffComponent.h"
#include "Misc/SlimePitComponent.h"
#include "Misc/SlimePitDeathSpawnComponent.h"
#include "Misc/TeleportPlayerToSpawnOnHitComponent.h"
#include "Misc/SetToStartOnAwakeFixComponent.h"

/////////////////////////////////
//        Player Logic         //
/////////////////////////////////

#include "PlayerLogic/PlayerIdComponent.h"
#include "PlayerLogic/PlayerSpawnPointComponent.h"
#include "PlayerLogic/PlayerDownedObjectComponent.h"
#include "PlayerLogic/PlayerDeathCameraAnimatorComponent.h"
#include "PlayerLogic/CharacterController/CharacterControllerComponent.h"
#include "PlayerLogic/AnimationControllers/PlayerGunAnimationControllerComponent.h"
#include "PlayerLogic/AnimationControllers/PlayerAnimationControllerComponent.h"
#include "PlayerLogic/AnimationControllers/FacePlayerBoneTowardsMeComponent.h"

#include "PlayerLogic/LookAtSystem/PlayerLookAtComponent.h"

/////////////////////////////////
//        Damage System        //
/////////////////////////////////

#include "DamageSystem/ExplosionComponent.h"
#include "DamageSystem/CritSpotComponent.h"
#include "DamageSystem/DamageOnCollideComponent.h"
#include "DamageSystem/DamageTextComponent.h"
#include "DamageSystem/DamageTextManagerComponent.h"

/////////////////////////////////
//    Interaction System       //
/////////////////////////////////

#include "InteractionSystem/InteractionBayComponent.h"
#include "InteractionSystem/InteractableComponent.h"
#include "InteractionSystem/Pickups/WeaponPickup.h"
#include "InteractionSystem/Triggers/ControlPointComponent.h"

/////////////////////////////////
//          Triggers           //
/////////////////////////////////

#include "InteractionSystem/RaidTriggers/EmpowerTriggerComponent.h"
#include "InteractionSystem/RaidTriggers/DisempowerTriggerComponent.h"
#include "InteractionSystem/Triggers/RevivePlayerComponent.h"

/////////////////////////////////
//        Weapon System        //
/////////////////////////////////

#include "WeaponSystem/Weapons/BaseWeaponComponent.h"
#include "WeaponSystem/Weapons/HitscanWeaponComponent.h"
#include "WeaponSystem/AmmoPickUpComponent.h"
#include "WeaponSystem/Weapons/FirePosComponent.h"
#include "WeaponSystem/Weapons/WeaponPosComponent.h"
#include "WeaponSystem/Inventory/InventoryComponent.h"
#include "WeaponSystem/Projectiles/ProjectileComponent.h"
#include "WeaponSystem/RaycastComponent.h"

/////////////////////////////////
//        Trails System        //
/////////////////////////////////

#include "Trails/TrailComponent.h"

/////////////////////////////////
//            AI               //
/////////////////////////////////

#include "AI/WaypointComponent.h"
#include "AI/WaypointAgentComponent.h"
#include "AI/WaypointManagerComponent.h"
#include "AI/AIMovementControllerComponent.h"
#include "AI/AIHordeReasoner.h"

#include "EnemyDeathComponent.h"

#include "BossAIComponent.h"
#include "Boss/Components/SludgeshotProjectileComponent.h"
#include "Boss/Components/SeedshotProjectileComponent.h"
#include "Boss/Components/SludgeshotAoeComponent.h"
#include "VineAIComponent.h"
#include "VineSpawnerComponent.h"

#include "AIHordelingTypeComponent.h"
#include "FodderAIComponent.h"
#include "TankAIComponent.h"
#include "BomberAIComponent.h"

/////////////////////////////////
//       Level Logic           //
/////////////////////////////////

#include "LevelSegmentManager/LevelSegmentManagerComponent.h"

#include "LevelSegmentManager/SegmentResources/UnloadResourceComponent.h"
#include "LevelSegmentManager/SegmentResources/TutorialResourcesComponent.h"
#include "LevelSegmentManager/SegmentResources/CombatBowl1ResourcesComponent.h"
#include "LevelSegmentManager/SegmentResources/BossRoomResourcesComponent.h"
#include "LevelSegmentmanager/SegmentResources/EndingCreditsResourcesComponent.h"

/////////////////////////////////
//       Entity Animation      //
/////////////////////////////////

#include "EntityAnimation/EntityAnimatorComponent.h"
#include "EntityAnimation/EntityAnimatorGroupComponent.h"
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
#include "FX/PlayClipOnAnimationFinishComponent.h"
#include "FX/FaceBoneTowardsTargetComponent.h"
#include "FX/LightingGroupComponent.h"
#include "FX/ShieldFXComponent.h"
#include "FX/PlayerShieldFXComponent.h"
#include "FX/TextureAnimatorComponent.h"
#include "FX/PlayAudioEventComponent.h"
#include "FX/PlayAudioEventOnEntityAnimationFinishComponent.h"
#include "FX/PlayAudioEventOnTriggerComponent.h"

/////////////////////////////////
//             Misc            //
/////////////////////////////////

#include "Misc/AnimateTutorialTextComponent.h"
#include "Misc/DeathTriggerGroupComponent.h"
#include "LevelSegmentManager/SegmentLogic/Misc/AnimateOnSegmentComponent.h"
#include "LevelSegmentManager/SegmentLogic/Misc/ChangeSegmentOnAnimatorFinishComponent.h"
#include "LevelSegmentManager/SegmentLogic/Misc/Phase3BossSpawnComponent.h"
#include "LevelSegmentManager/SegmentLogic/Misc/BossBattleMusicManagerComponent.h"
#include "LevelSegmentManager/SegmentLogic/Misc/TutorialDialogueManagerComponent.h"
#include "Misc/DeathTriggerGroupComponent.h"
#include "Misc/PingTestComponent.h"

/////////////////////////////////
//           Physics           //
/////////////////////////////////

#include "Physics/ApplyImpulseOnHitComponent.h"

/////////////////////////////////
//         Triggers            //
/////////////////////////////////

#include "Triggers/SegmentChangeTriggerComponent.h"
#include "Triggers/VisibilityTriggerComponent.h"
#include "Triggers/AnimateEntityOnTriggerComponent.h"
#include "Triggers/Phase3LightTriggerComponent.h"

/////////////////////////////////
//           UI                //
/////////////////////////////////

#include "UI/UIScreensConfigComponent.h"

