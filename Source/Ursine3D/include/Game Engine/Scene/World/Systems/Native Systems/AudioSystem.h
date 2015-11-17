/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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

		class AudioSystem : EntitySystem
		{
			ENTITY_SYSTEM;

		public:
			AudioSystem(World *world);

			void CreateAudioObject(AkGameObjectID& id);

		private:
			AudioManager* m_audioMan;

			std::unordered_map<EntityUniqueID, ursine::ecs::AudioEmitter*> m_emitters;
			std::unordered_map<std::string, AkBankID> m_banks;

			AkGameObjectID m_nextEmptyID;

			void OnInitialize(void) override;
			void OnRemove(void) override;

			void onComponentAdded(EVENT_HANDLER(World));
			void onComponentRemoved(EVENT_HANDLER(World));

			void onUpdate(EVENT_HANDLER(World));

		} Meta(Enable);
	}
}