#pragma once

#include "AudioManager.h"
#include "SVec3.h"

namespace ursine
{
	namespace Audio
	{
		class Emitter
		{
		public:
			Emitter();

			Emitter(AkUInt32 listeners);

			Emitter(const SVec3 pos, const SVec3 orient);

			Emitter(AkUInt32 listeners, const SVec3 pos, const SVec3 orient);

			AkGameObjectID GetID();

			std::vector<std::string> &GetEventList();

			void AddEvent(const std::string event);

			void UpdatePosition(const SVec3 pos, const SVec3 orient);

			SVec3& GetPosition();

			SVec3& GetOrientation();

		private:
			AkGameObjectID m_objID;

			std::vector<std::string> m_eventsList;

			SVec3 m_position;

			SVec3 m_orientation;
		};
	}
}