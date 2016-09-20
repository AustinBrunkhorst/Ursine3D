// create class

// override "CanExit", and return m_triggered.

// overrides "Enter", so that it can subscribe to the trigger entity's collision events

// No default constructor, only a constructor that takes a string name of an entity.

/* ---------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** TriggerWaitState.h
**
** Author Jason Burch- j.orion@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "SegmentLogicState.h"

class TriggerWaitState : public SegmentLogicState
{
public:

	TriggerWaitState( std::string& name );

	void Enter(SegmentLogicStateMachine *machine) override;

private:
	bool m_triggered;
	std::string m_entityName;

	void onTrigger(EVENT_HANDLER(ursine::ecs::World));
};