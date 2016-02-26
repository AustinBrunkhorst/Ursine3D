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
//       Camera Animation      //
/////////////////////////////////

#include "CameraAnimation/CameraAnimatorSystem.h"

/////////////////////////////////
//       Entity Animation      //
/////////////////////////////////

#include "EntityAnimation/EntityAnimatorSystem.h"

/////////////////////////////////
//       Spawner System        //
/////////////////////////////////

#include "SpawnerSystem/SpawnerGroupSystem.h"

/////////////////////////////////
//       Level Events          //
/////////////////////////////////

#include "LevelSegmentManager/SegmentLogic/Misc/StartRoomDirectorSystem.h"
