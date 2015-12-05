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

		float m_roundTimer;
		int m_roundNumber;
		bool m_running;

		void OnInitialize(void) override;
		void OnRemove(void) override;

		Meta(Disable)
		void onUpdate(EVENT_HANDLER(ecs::World));
	} Meta(Enable);
}
