/* ----------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** DeathTriggerGroupComponent.h
**
** Author:
** - Jason Burch - j.orion@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include "LevelSegments.h"

class DeathTriggerGroup : public ursine::ecs::Component
{
	NATIVE_COMPONENT;

public:

	EditorField(
		LevelSegments changeNewSegment,
		GetNewLevelSegment,
		SetNewLevelSegment
		);

	DeathTriggerGroup(void);

	LevelSegments GetNewLevelSegment(void) const;
	void SetNewLevelSegment(LevelSegments levelSegment);

	void OnInitialize(void) override;

private:
	LevelSegments m_newSegment;
	int m_numAlive;

	void onUpdate(EVENT_HANDLER(World));
	void onTargetDead(EVENT_HANDLER(Health));

} Meta(Enable);