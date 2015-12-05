#pragma once

#include <Component.h>

namespace ursine
{
	// This is the recorder component for all entities that are recording their commands
	class Recorder : public ecs::Component
	{
	public:
		// Enum for this recorder's state (recording, stopped, playback)
		// ID for the recording index
		// Timer for the current recording time

	private:
	};
}
