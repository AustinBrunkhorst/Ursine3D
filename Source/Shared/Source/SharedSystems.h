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

#include "Systems/CheatSystem.h"
#include "Systems/SpawnSystem.h"
#include "Systems/RoundSystem.h"
#include "Systems/TimeDeathSystem.h"
#include "Systems/ControlPointSystem.h"

/////////////////////////////////
//    Character Controller     //
/////////////////////////////////

#include "CharacterController/CharacterControllerSystem.h"

/////////////////////////////////
//     Interaction System      //
/////////////////////////////////

#include "InteractionSystem/InteractionBaySystem.h"

/////////////////////////////////
//        Weapon System        //
/////////////////////////////////

#include "WeaponSystem/BaseWeaponSystem.h"
#include "WeaponSystem/Projectiles/ProjectileSystem.h"
#include "WeaponSystem/InventorySystem.h"

/////////////////////////////////
//        Command System       //
/////////////////////////////////

#include "CommandSystem/CommandInputControllerSystem.h"
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
//       Spawner System        //
/////////////////////////////////

#include "SpawnerSystem/SpawnerGroupSystem.h"

/////////////////////////////////
//       Level Events          //
/////////////////////////////////

#include "LevelEvents/Logic/StartRoomDirectorSystem.h"
