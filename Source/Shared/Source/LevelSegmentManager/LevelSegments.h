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
        Tut_OpeningCinematic,
        Tut_MovementTutorial,
        Tut_SoloTriggerTutorial,
        Tut_WeaponPickupTutorial,
        Tut_HipFireTutorial,
        Tut_AimFireTutorial,
        Tut_AmmoPickupTutorial,
        Tut_ShootMovingTargetsTutorial,
        Tut_ReviveTutorial,
        Tut_SimultaneousTriggerTutorial,

        // Combat Bowl 1
        CB1_SimulationCreationCinematic,
        CB1_SimulationStartCinematic,
        CB1_WeaponSelection,
        CB1_ActivateSystems1,
        CB1_Combat1, // Fodder introduced
        CB1_Combat2, // Tanks introduced
        CB1_Combat3, // Fodder + Tanks combat integration
        CB1_Combat4, // Combat test
        CB1_OpenConduitRoom,

        // Conduit Tutorial
        Conduit_Part1, // traversal and / or platforming enter
        Conduit_Part2, // Activate system
        Conduit_Part3, // Conduit introduced
        Conduit_Part4, // Conduit test
        Conduit_Part5, // Traversal and / or platforming exit

        // Combat Bowl 2
        CB2_UnlockWeaponCase,
        CB2_ActivateSystem1,
        CB2_Combat1, // Nuker introduced
        CB2_Combat2, // Nuker combat integration
        CB2_ActivateSystem2,
        CB2_Combat3, // Nuker combat test
        CB2_Combat4, // Combat w/ conduit
        CB2_OpenEmpowerDisempowerRoom,

        // Empower / Disempower tutorial
        EmpDis_Part1, // Traversal and / or platforming enter
        EmpDis_Part2, // Activate system
        EmpDis_Part3, // Empower introduced
        EmpDis_Part4, // Empower combat test
        EmpDis_Part5, // Activate system
        EmpDis_Part6, // Disempower introduced
        EmpDis_Part7, // Disempower combat test
        EmpDis_Part8, // Activate system
        EmpDis_Part9, // Empower / Disempower
        EmpDis_Part10, // Empower / Disempower combat test
        EmpDis_Part11, // Traversal and / or platforming exit

        // Combat Bowl 3
        CB3_UnlockWeaponCase,
        CB3_ActivateSystem1,
        CB3_Combat1, // Bomber introduced
        CB3_ActivateSystem2,
        CB3_Combat2, // Bomber combat integration
        CB3_Combat3, // Bomber combat test
        CB3_ActivateSystem3,
        CB3_Combat4, // Combat w/ empower
        CB3_OpenAccumulateRoom,

        // Accumulate Tutorial
        Accum_Part1, // Traversal and / or platforming enter
        Accum_Part2, // Activate system
        Accum_Part3, // Accumulate introduced [use environment hazzard]
        Accum_Part4, // Activate system 2 [turns hazard off]
        Accum_Part5, // Accumulate test
        Accum_Part6, // Traversal and / or platforming exit

        // Combat Bowl 4
        CB4_UnlockWeaponCase,
        CB4_ActivateSystem1,
        CB4_Combat1, // Agile introduced
        CB4_ActivateSystem2,
        CB4_combat2, // Agile combat integration
        CB4_Combat3, // Agile combat test
        CB4_ActivateSystem3,
        CB4_Combat4, // Cumulate enemy test
        CB4_ActivateSystem4,
        CB4_Combat5, // Combat w/ empower
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
