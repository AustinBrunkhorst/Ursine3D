/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AudioSystem.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "AudioSystem.h"
#include "ListenerComponent.h"

namespace ursine
{
	namespace ecs
	{
		ENTITY_SYSTEM_DEFINITION(AudioSystem);

		AudioSystem::AudioSystem(World *world)
			: EntitySystem(world)
			, m_nextEmptyID(AUDIO_UNALLOCATED_ID + 1)
		{
			m_audioMan = GetCoreSystem(AudioManager);
		}

		void AudioSystem::CreateAudioObject(AkGameObjectID& id)
		{
			id = m_nextEmptyID++;
			m_audioMan->RegisterObject(id, 0);
		}

		void AudioSystem::DeleteAudioObject(AkGameObjectID& id)
		{
			m_audioMan->UnRegisterObject(id);
		}

		void AudioSystem::OnInitialize(void)
		{
			m_world->Listener(this)
				.On(WORLD_UPDATE, &AudioSystem::onUpdate)
				.On(WORLD_ENTITY_COMPONENT_ADDED, &AudioSystem::onComponentAdded)
				.On(WORLD_ENTITY_COMPONENT_REMOVED, &AudioSystem::onComponentRemoved);
		}

		void AudioSystem::OnRemove(void)
		{
			m_world->Listener(this)
				.Off(WORLD_UPDATE, &AudioSystem::onUpdate)
				.Off(WORLD_ENTITY_COMPONENT_ADDED, &AudioSystem::onComponentAdded)
				.Off(WORLD_ENTITY_COMPONENT_REMOVED, &AudioSystem::onComponentRemoved);
		}

		void AudioSystem::onComponentAdded(EVENT_HANDLER(World))
		{
			EVENT_ATTRS(World, ComponentEventArgs);

			if (args->component->Is<AudioEmitter>())
			{
				m_emitters.emplace(
					args->entity->GetID( ),
					static_cast<AudioEmitter*>(const_cast<Component*>(args->component))
				);
				auto& handle = m_emitters[args->entity->GetID()]->m_handle;
				CreateAudioObject(handle);
				AssignListener(handle, m_emitters[args->entity->GetID()]->GetListeners());
			}

			else if (args->component->Is<AudioListener>())
			{
				m_listeners.emplace(
					args->entity->GetID(),
					static_cast<AudioListener*>(const_cast<Component*>(args->component))
					);
				m_listeners[args->entity->GetID()]->SetListenerIndex(m_audioMan->GetListener());
			}
		}

		void AudioSystem::onComponentRemoved(EVENT_HANDLER(World))
		{
			EVENT_ATTRS(World, ComponentEventArgs);

			if (args->component->Is<AudioEmitter>())
			{
				auto search = m_emitters.find(args->entity->GetID());

				if (search != m_emitters.end())
				{
					DeleteAudioObject(m_emitters[args->entity->GetID()]->m_handle);
					m_emitters.erase(search);
				}
			}

			else if (args->component->Is<AudioListener>())
			{
				auto search = m_listeners.find(args->entity->GetID());

				if (search != m_listeners.end())
				{
					m_audioMan->FreeListener(m_listeners[args->entity->GetID()]->GetListenerIndex());
					m_listeners.erase(search);
				}
					
			}
		}

		void AudioSystem::onUpdate(EVENT_HANDLER(World))
		{
			// walk through all emitters
			// update positions
			// Post Events
			for (auto &emitter : m_emitters)
			{
				auto mute = emitter.second->GetMute();

				if (mute)
					continue;

				auto& dirty = emitter.second->m_dirty;

				auto& handle = emitter.second->m_handle;
				auto entity = m_world->GetEntity(emitter.first);

				if (!entity)
					continue;

				auto trans = entity->GetTransform();

				if (dirty)
					SetObject3DPosition(handle, trans->GetWorldPosition(), 
						trans->GetForward());

				while (!emitter.second->SoundsEmpty())
				{
					m_audioMan->PlayEvent(emitter.second->GetFrontSound(), handle);
					emitter.second->PopFrontSound();
				}
			}

			for (auto &listener : m_listeners)
			{
				auto& dirty = listener.second->m_dirty;

				auto index = listener.second->GetListenerIndex();

				auto entity = m_world->GetEntity(listener.first);

				if (!entity)
					return;

				auto trans = entity->GetTransform();

				if (dirty)
					SetListener3DPosition(trans->GetForward(), trans->GetUp(), 
						trans->GetWorldPosition(), index);
			}
		}

		void AudioSystem::SetRealTimeParameter(const std::string param, const float value, AkGameObjectID id)
		{
			if (AK::SoundEngine::SetRTPCValue(param.c_str(), (AkRtpcValue)value, id) != AK_Success)
			{
				UWarning("Wwise: Cannot Set RTPC value: %s", param.c_str());
			}
		}

		void AudioSystem::AssignListener(AkGameObjectID obj, ListenerIndex listeners)
		{
			if (listeners == ListenerIndex::None)
			{
				UWarning("Wwise: Cannot Set Zero Active Listeners");
				return;
			}

			if (AK::SoundEngine::SetActiveListeners(obj, static_cast<AkUInt32>(listeners)) != AK_Success)
			{
				UWarning("Wwise: Cannot Set Active Listeners");
			}
		}

		void AudioSystem::SetListener3DPosition(const AkVector orientation_forward,
			const AkVector orientation_up, const AkVector position, const ListenerIndex listeners)
		{
			if (position.X == 0 &&
				position.Y == 0 &&
				position.Z == 0)
				return;

			AkListenerPosition listenerPosition;
			// up and forward have to be orthogonal and normalized
			listenerPosition.OrientationTop = orientation_up;
			listenerPosition.OrientationFront = orientation_forward;
			listenerPosition.Position = position;

			if (AK::SoundEngine::SetListenerPosition(listenerPosition, static_cast<AkUInt32>(listeners)) != AK_Success)
			{
				UWarning("Wwise: Cannot Set Listener Postion");
			}
		}

		void AudioSystem::SetListener3DPosition(const SVec3 orientation_forward,
			const SVec3 orientation_up, const SVec3 position, const ListenerIndex listeners)
		{
			if (position.X() == 0 &&
				position.Y() == 0 &&
				position.Z() == 0)
				return;

			AkListenerPosition listenerPosition;
			// up and forward have to be orthogonal and normalized
			listenerPosition.OrientationTop = orientation_up.ToWwise();
			listenerPosition.OrientationFront = orientation_forward.ToWwise();
			listenerPosition.Position = position.ToWwise();

			if (AK::SoundEngine::SetListenerPosition(listenerPosition, static_cast<AkUInt32>(listeners)) != AK_Success)
			{
				UWarning("Wwise: Cannot Set Listener Postion");
			}
		}

		void AudioSystem::SetObject3DPosition(AkGameObjectID obj, const AkSoundPosition position)
		{
			if (AK::SoundEngine::SetPosition(obj, position) != AK_Success)
			{
				UWarning("Wwise: Cannot Set Object 3D Position");
			}
		}

		void AudioSystem::SetMultipleObject3DPosition(AkGameObjectID obj, const AkSoundPosition* positions,
			AkUInt16 num_positions, AK::SoundEngine::MultiPositionType type)
		{
			// MultiPositionType_MultiSources 	
			// Simulate multiple sources in one sound playing, adding volumes. 
			// For instance, all the torches on your level emitting using only one sound.

			// MultiPositionType_MultiDirections
			// Simulate one sound coming from multiple directions. Useful for repositioning 
			// sounds based on wall openings or to simulate areas like forest or rivers

			if (SetMultiplePositions(obj, positions, num_positions, type) != AK_Success)
			{
				UWarning("Wwise: Cannot Set Mutiple Object 3D Position");
			}
		}

		void AudioSystem::SetSoundObstructionAndOcclusion(AkGameObjectID obstruction,
			const AkUInt32 listeners, const AkReal32 obstruction_level, const AkReal32 occlusion_level)
		{
			if (AK::SoundEngine::SetObjectObstructionAndOcclusion(obstruction, listeners,
				obstruction_level, occlusion_level) != AK_Success)
			{
				UWarning("Wwise: Cannot Set Obstruction and Occlusion");
			}
		}

		void AudioSystem::SetGameState(const std::string name, const std::string state)
		{
			// global state change
			if (AK::SoundEngine::SetState(name.c_str(), state.c_str()))
			{
				UWarning("Wwise: Cannot Set State");
			}
		}

		void AudioSystem::SetObjectSwitch(const std::string name, const std::string state, AkGameObjectID obj)
		{
			// specific states for each object
			if (AK::SoundEngine::SetSwitch(name.c_str(), state.c_str(), obj))
			{
				UWarning("Wwise: Cannot Set Switch");
			}
		}

		void AudioSystem::SetTrigger(const std::string name, AkGameObjectID obj)
		{
			if (AK::SoundEngine::PostTrigger(name.c_str(), obj))
			{
				UWarning("Wwise: Cannot Post Trigger");
			}
		}

		void AudioSystem::SetObject3DPosition(AkGameObjectID obj, const SVec3 position, const SVec3 orientation)
		{
			AkSoundPosition pos;
			pos.Position = position.ToWwise();
			pos.Orientation = orientation.ToWwise();
			if (AK::SoundEngine::SetPosition(obj, pos) != AK_Success)
			{
				UWarning("Wwise: Cannot Set Object 3D Position");
			}
		}
	}
}