/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** LevelManagerSystem.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** --------------------------------------------------------------------------*/

#pragma once

#include <EntitySystem.h>
#include <EventDispatcher.h>

#include "LevelSegments.h"
#include "SegmentLogicStateMachine.h"

class LevelSegmentChangeArgs : public ursine::EventArgs
{
public:
    LevelSegments segment;
};

enum class LevelSegmentManagerEvents
{
    SegmentChanged
};

class LevelSegmentManager 
    : public ursine::ecs::EntitySystem
    , public ursine::EventDispatcher<LevelSegmentManagerEvents>
{
    ENTITY_SYSTEM;

public:

    LevelSegmentManager(ursine::ecs::World *world);
    ~LevelSegmentManager(void);

    void SegmentTransition(LevelSegments segment);

private:

    // All the state machines for the segments of gameplay
    std::vector<SegmentLogicStateMachine::Handle> m_segmentLogic[ LevelSegments::Empty ];

    LevelSegments m_segment;

    void OnAfterLoad(void) override;

    void initTutorialLogic(void);
    
    void initCombatBowl1Logic(void);
    void initConduitTutLogic(void);
    
    void initCombatBowl2Logic(void);
    void initEmpowerDisempowerTutLogic(void);
    
    void initCombatBowl3Logic(void);
    void initAccumulateTutLogic(void);
    
    void initCombatBowl4Logic(void);
    void initBossRoomLogic(void);

    // Create a state machine given a name and a segment to update it for
    SegmentLogicStateMachine::Handle createSegmentLogic(const std::string &name, LevelSegments segment);

    // Add logic to a segment given a segment and a state machine already created
    void addSegmentLogic(LevelSegments segment, SegmentLogicStateMachine::Handle logic);

    void onUpdate(EVENT_HANDLER(World));

} Meta(Enable, AutoAddEntitySystem);
