/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** LevelEvents.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <Meta.h>

namespace LevelSegments_enum
{
    enum LevelSegments
    {
        // Tutorial
        Tut_OpeningCinematic, // Introduce players?  What about dome
        Tut_GateOpens,
        Tut_MovementTutorial, // Tell the player to move to a position
        Tut_JumpTutorial, // Tell the player to jump
        Tut_WeaponPickupTutorial, // How to pickup weapons (weapon pickup + FX + button icon)
        Tut_HipFireTutorial, // Shooting targets
        Tut_AimFireTutorial, // Shooting targets
        Tut_AmmoPickupTutorial, // Ammo pickups
        Tut_ShootMovingTargetsTutorial, // Shooting moving targets
        Tut_SoloTriggerTutorial, // Accumulate trigger pad (particle effects)
        Tut_ReviveTutorial, // Revive mechanic (kill one player, have other revive the other)
        Tut_SimultaneousTriggerTutorial, // SoloTrigger X 2
        Tut_SimulationCreationCinematic, // Cinematic for simulation begin

        // Combat Bowl 1
        Meta(Disable)
        CB1_SimulationStartCinematic,
        Meta(Disable)
        CB1_WeaponSelection,
        Meta(Disable)
        CB1_ActivateSystems1,
        Meta(Disable)
        CB1_Combat1, // Fodder introduced
        Meta(Disable)
        CB1_Combat2, // Tanks introduced
        Meta(Disable)
        CB1_Combat3, // Fodder + Tanks combat integration
        Meta(Disable)
        CB1_Combat4, // Combat test
        Meta(Disable)
        CB1_OpenConduitRoom,

        // Conduit Tutorial
        Meta(Disable)
        Conduit_Part1, // traversal and / or platforming enter
        Meta(Disable)
        Conduit_Part2, // Activate system
        Meta(Disable)
        Conduit_Part3, // Conduit introduced
        Meta(Disable)
        Conduit_Part4, // Conduit test
        Meta(Disable)
        Conduit_Part5, // Traversal and / or platforming exit

        // Combat Bowl 2
        Meta(Disable)
        CB2_UnlockWeaponCase,
        Meta(Disable)
        CB2_ActivateSystem1,
        Meta(Disable)
        CB2_Combat1, // Nuker introduced
        Meta(Disable)
        CB2_Combat2, // Nuker combat integration
        Meta(Disable)
        CB2_ActivateSystem2,
        Meta(Disable)
        CB2_Combat3, // Nuker combat test
        Meta(Disable)
        CB2_Combat4, // Combat w/ conduit
        Meta(Disable)
        CB2_OpenEmpowerDisempowerRoom,

        // Empower / Disempower tutorial
        Meta(Disable)
        EmpDis_Part1, // Traversal and / or platforming enter
        Meta(Disable)
        EmpDis_Part2, // Activate system
        Meta(Disable)
        EmpDis_Part3, // Empower introduced
        Meta(Disable)
        EmpDis_Part4, // Empower combat test
        Meta(Disable)
        EmpDis_Part5, // Activate system
        Meta(Disable)
        EmpDis_Part6, // Disempower introduced
        Meta(Disable)
        EmpDis_Part7, // Disempower combat test
        Meta(Disable)
        EmpDis_Part8, // Activate system
        Meta(Disable)
        EmpDis_Part9, // Empower / Disempower
        Meta(Disable)
        EmpDis_Part10, // Empower / Disempower combat test
        Meta(Disable)
        EmpDis_Part11, // Traversal and / or platforming exit

        // Combat Bowl 3
        Meta(Disable)
        CB3_UnlockWeaponCase,
        Meta(Disable)
        CB3_ActivateSystem1,
        Meta(Disable)
        CB3_Combat1, // Bomber introduced
        Meta(Disable)
        CB3_ActivateSystem2,
        Meta(Disable)
        CB3_Combat2, // Bomber combat integration
        Meta(Disable)
        CB3_Combat3, // Bomber combat test
        Meta(Disable)
        CB3_ActivateSystem3,
        Meta(Disable)
        CB3_Combat4, // Combat w/ empower
        Meta(Disable)
        CB3_OpenAccumulateRoom,

        // Accumulate Tutorial
        Meta(Disable)
        Accum_Part1, // Traversal and / or platforming enter
        Meta(Disable)
        Accum_Part2, // Activate system
        Meta(Disable)
        Accum_Part3, // Accumulate introduced [use environment hazzard]
        Meta(Disable)
        Accum_Part4, // Activate system 2 [turns hazard off]
        Meta(Disable)
        Accum_Part5, // Accumulate test
        Meta(Disable)
        Accum_Part6, // Traversal and / or platforming exit

        // Combat Bowl 4
        Meta(Disable)
        CB4_UnlockWeaponCase,
        Meta(Disable)
        CB4_ActivateSystem1,
        Meta(Disable)
        CB4_Combat1, // Agile introduced
        Meta(Disable)
        CB4_ActivateSystem2,
        Meta(Disable)
        CB4_combat2, // Agile combat integration
        Meta(Disable)
        CB4_Combat3, // Agile combat test
        Meta(Disable)
        CB4_ActivateSystem3,
        Meta(Disable)
        CB4_Combat4, // Cumulate enemy test
        Meta(Disable)
        CB4_ActivateSystem4,
        Meta(Disable)
        CB4_Combat5, // Combat w/ empower
        Meta(Disable)
        CB4_OpenBossRoom,

        // Boss Room
        BossRoom_Platforming,
        BossRoom_Introduction,
        BossRoom_Phase1, // Warm up
        BossRoom_Phase2, // Escalation, conduit
        BossRoom_Phase3, // Mid-point, empower / disempower
        BossRoom_Phase4, // Climax, accumulate
        BossRoom_Phase5, // Kill sequence

        Empty
    } Meta(Enable, DisplayName("LevelSegments"));
}

typedef LevelSegments_enum::LevelSegments LevelSegments;
