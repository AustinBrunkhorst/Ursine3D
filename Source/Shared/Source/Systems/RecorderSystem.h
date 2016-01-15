/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RecorderSystem.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <EntitySystem.h>
#include "Recording.h"
#include "TeamComponent.h"

namespace ursine
{
	class RecorderSystem : public ecs::EntitySystem
	{
		ENTITY_SYSTEM;

	public:
		RecorderSystem(ursine::ecs::World *world);

		// This function is called at the start of each round.
		// The parameter is an array of team members
		Meta(Disable)
		void SetRoundStart(std::vector<std::vector<TeamComponent*> > &teamCompnents);

		Meta(Disable)
		void RecordCommand(RecordableCommandPtr command, ecs::Entity *entity);

	private:

		// Index into this 2D array usig [teamNumber][liveNumber]
		Meta(Disable)
		std::vector<std::vector<std::pair<ecs::EntityUniqueID, Recording> > > m_recordings;

		uint64 m_roundTimer;
		int m_roundNumber;
		bool m_running;

		void OnInitialize(void) override;
		void OnRemove(void) override;

		Meta(Disable)
		void onUpdate(EVENT_HANDLER(ecs::World));

	} Meta(Enable);
}
