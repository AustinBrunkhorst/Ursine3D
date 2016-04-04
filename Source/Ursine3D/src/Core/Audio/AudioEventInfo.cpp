/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AudioEventInfo.cpp
**
** Author:
** - Jason Burch - j.orion@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "AudioEventInfo.h"

#include "AudioSystem.h"

namespace ursine
{
	namespace ecs
	{
		void AudioSwitch::ApplyParams(AkGameObjectID id)
		{
			AudioSystem::SetObjectSwitch( switchGroup, switchState, id );
		}

		void AudioState::ApplyParams(AkGameObjectID id)
		{
			AudioSystem::SetGameState( stateGroup, audioState );
		}

		void AudioRTPC::ApplyParams(AkGameObjectID id)
		{
			AudioSystem::SetRealTimeParameter( parameter, val, id );
		}

		void AudioTrigger::ApplyParams(AkGameObjectID id)
		{
			AudioSystem::SetTrigger( name, id );
		}

		void AudioGeneric::ApplyParams(AkGameObjectID id)
		{
			AudioSystem::PostAudioEvent( name, id );
		}
	}
}