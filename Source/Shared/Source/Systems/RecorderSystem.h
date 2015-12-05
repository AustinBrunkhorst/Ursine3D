#pragma once

#include <FilterSystem.h>

namespace ursine
{
	class RecorderSystem : public ecs::FilterSystem
	{
	public:

	private:
		std::vector<std::vector<Recording>> m_recordings;
	};
}
