/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** SoundFrameClient.h
**
** Author:
** - Jason Burch - j.orion@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <AK/SoundFrame/SF.h>

namespace ursine
{
	class SoundFrameClient
		: public AK::SoundFrame::IClient
	{
	public:

		SoundFrameClient();

		~SoundFrameClient();

		// SoundFrame::IClient methods

		virtual void OnConnect(bool in_bConnect) {}
		virtual void OnEventNotif(Notif in_eNotif, AkUniqueID in_eventID) {}
		virtual void OnDialogueEventNotif(Notif in_eNotif, AkUniqueID in_dialogueEventID) {}
		virtual void OnSoundObjectNotif(Notif in_eNotif, AkUniqueID in_soundObjectID) {}
		virtual void OnStatesNotif(Notif in_eNotif, AkUniqueID in_stateGroupID) {}
		virtual void OnSwitchesNotif(Notif in_eNotif, AkUniqueID in_switchGroupID) {}
		virtual void OnGameParametersNotif(Notif in_eNotif, AkUniqueID in_gameParameterID) {}
		virtual void OnTriggersNotif(Notif in_eNotif, AkUniqueID in_triggerID) {}
		virtual void OnArgumentsNotif(Notif in_eNotif, AkUniqueID in_argumentID) {}
		virtual void OnEnvironmentsNotif(Notif in_eNotif, AkUniqueID in_environmentID) {}
		virtual void OnGameObjectsNotif(Notif in_eNotif, AkGameObjectID in_gameObjectID) {}

	private:
		AK::SoundFrame::ISoundFrame * m_pSoundFrame;
	};
}