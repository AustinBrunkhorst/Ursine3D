/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PhysicsSystem.h
**
** Author:
** - Jason Burch - j.orion@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "EntitySystem.h"
#include "AudioManager.h"
#include "AudioID.h"

namespace ursine
{
	namespace ecs
	{
		class AudioEmitter;
		class AudioListener;

		class AudioSystem : EntitySystem
		{
			ENTITY_SYSTEM;

		public:
			Meta(Enable)
			AudioSystem(World *world);

			void CreateAudioObject(AkGameObjectID& id);

			void DeleteAudioObject(AkGameObjectID& id);

			static void SetRealTimeParameter(const std::string param, const float value, AkGameObjectID id);

			void AssignListener(AkGameObjectID obj, ListenerIndex listeners);

			void SetListener3DPosition(const AkVector orientation_forward,
				const AkVector orientation_up, const AkVector position, const ListenerIndex listeners);

			void SetListener3DPosition(const SVec3 orientation_forward,
				const SVec3 orientation_up, const SVec3 position, const ListenerIndex listeners);

			void SetObject3DPosition(AkGameObjectID obj, const AkSoundPosition position);

			void SetObject3DPosition(AkGameObjectID obj, const SVec3 position, const SVec3 orientation);

			void SetMultipleObject3DPosition(AkGameObjectID obj, const AkSoundPosition* positions,
				AkUInt16 num_positions, AK::SoundEngine::MultiPositionType type);

			void SetSoundObstructionAndOcclusion(AkGameObjectID obstruction,
				const AkUInt32 listeners, const AkReal32 obstruction_level, const AkReal32 occlusion_level);

			static void SetGameState(const std::string name, const std::string state);
            
			static void SetObjectSwitch(const std::string name, const std::string state, AkGameObjectID obj);

			static void SetTrigger(const std::string name, AkGameObjectID obj);

		private:
			AudioManager *m_audioMan;

			std::unordered_map<EntityID, AudioEmitter*> m_emitters;
			std::unordered_map<EntityID, AudioListener*> m_listeners;
			std::unordered_map<std::string, AkBankID> m_banks;

			AkGameObjectID m_nextEmptyID;

			void OnInitialize(void) override;
			void OnRemove(void) override;

			void onComponentAdded(EVENT_HANDLER(World));
			void onComponentRemoved(EVENT_HANDLER(World));

			void onUpdate(EVENT_HANDLER(World));

		} Meta(Disable, WhiteListMethods, AutoAddEntitySystem);
	}
}