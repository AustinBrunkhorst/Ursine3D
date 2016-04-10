/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SharedSystems.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

/////////////////////////////////
//        Game Management      //
/////////////////////////////////

#include "GameManagement/GameEntryPointSystem.h"

/////////////////////////////////
//        Miscelaneous         //
/////////////////////////////////

#include "Misc/CheatSystem.h"
#include "Misc/TimeDeathSystem.h"

/////////////////////////////////
//    Character Controller     //
/////////////////////////////////

#include "CharacterController/CharacterControllerSystem.h"

/////////////////////////////////
//     Interaction System      //
/////////////////////////////////

#include "InteractionSystem/InteractionBaySystem.h"
#include "InteractionSystem/Triggers/ControlPointSystem.h"

/////////////////////////////////
//        Weapon System        //
/////////////////////////////////

#include "WeaponSystem/Weapons/BaseWeaponSystem.h"
#include "WeaponSystem/Projectiles/ProjectileSystem.h"
#include "WeaponSystem/Inventory/InventorySystem.h"
#include "WeaponSystem/StickyAimSystem.h"

/////////////////////////////////
//        Trails System        //
/////////////////////////////////

#include "Trails/TrailSystem.h"

/////////////////////////////////
//        Command System       //
/////////////////////////////////

#include "CommandSystem/InputControllerSystem.h"
#include "CommandSystem/CommandQueueSystem.h"

/////////////////////////////////
//             AI              //
/////////////////////////////////

#include "AI/AIReasonerSystem.h"
#include "AI/AIControllerSystem.h"
#include "AI/WaypointSystem.h"

/////////////////////////////////
//       Entity Animation      //
/////////////////////////////////

#include "EntityAnimation/EntityAnimatorSystem.h"

/////////////////////////////////
//       Spawner System        //
/////////////////////////////////

#include "SpawnerSystem/SpawnerGroupSystem.h"

/////////////////////////////////
//       Damage System         //
/////////////////////////////////

//#include "DamageSystem/DamageTextSystem.h"
#include "DamageSystem/DamageOnCollideSystem.h"

/////////////////////////////////
//       Health System         //
/////////////////////////////////

#include "HealthSystem/HealthRegenSystem.h"
#include "FX/ShieldFXSystem.h"

