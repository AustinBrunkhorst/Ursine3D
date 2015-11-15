#include "UrsinePrecompiled.h"

#include "Emitter.h"

namespace ursine
{
	namespace Audio
	{
		Emitter::Emitter() : m_position(SVec3(0)), m_orientation(SVec3(1,0,0))
		{
			auto audio = GetCoreSystem(AudioManager);

			m_objID = audio->AssignSoundObjectID();

			audio->RegisterObject(m_objID, 0);
		}

		Emitter::Emitter(AkUInt32 listeners) : m_position(SVec3(0)), m_orientation(SVec3(1, 0, 0))
		{
			auto audio = GetCoreSystem(AudioManager);

			m_objID = audio->AssignSoundObjectID();

			audio->RegisterObject(m_objID, listeners);
		}

		Emitter::Emitter(const SVec3 pos, const SVec3 orient) : m_position(pos), m_orientation(orient)
		{
			auto audio = GetCoreSystem(AudioManager);

			m_objID = audio->AssignSoundObjectID();

			audio->RegisterObject(m_objID, 0);
		}

		Emitter::Emitter(AkUInt32 listeners, const SVec3 pos, const SVec3 orient) : m_position(pos), m_orientation(orient)
		{
			auto audio = GetCoreSystem(AudioManager);

			m_objID = audio->AssignSoundObjectID();

			audio->RegisterObject(m_objID, listeners);
		}

		AkGameObjectID Emitter::GetID()
		{
			
		}

		std::vector<std::string>& Emitter::GetEventList()
		{
			
		}

		void Emitter::AddEvent(const std::string event)
		{
			
		}

		void Emitter::UpdatePosition(const SVec3 pos, const SVec3 orient)
		{
			
		}

		SVec3& Emitter::GetPosition()
		{
			
		}

		SVec3& Emitter::GetOrientation()
		{
			
		}
	}
}