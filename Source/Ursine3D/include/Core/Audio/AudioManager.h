/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AudioManager.h
**
** Author:
** - Jason Burch - J.Orion@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "CoreSystem.h"
#include "NativeJSFunction.h"

#include "ListenerMasks.h"
#include "WwiseForward.h"
#include "WWiseUtils/AkFilePackageLowLevelIOBlocking.h"

#include "AudioData.h"

namespace ursine
{
	class AudioManager : public core::CoreSystem
	{
		CORE_SYSTEM;

	public:
		Meta(Enable)
		AudioManager(void) { }

		~AudioManager(void);

		void OnInitialize(void) override;
		void OnRemove(void) override;

		void PlayEvent(const std::string name, AkGameObjectID obj);

		static void PlayGlobalEvent(const std::string &name);
		static void StopGlobalEvent(const std::string &name);
		static bool IsGlobalEventPlaying(const std::string &name);

		static void PauseAudio(void);

		static void StopSound(std::string name, AkGameObjectID id);

		static void ResumeAudio(void);

		void LoadBank(const resources::AudioData &data, AkBankID &outInit, AkBankID &outBank);

		void UnloadBank(const resources::AudioData &data);

		void RegisterObject(AkGameObjectID obj, int listener);

		void UnRegisterObject(AkGameObjectID obj);

		bool GetListenerAvailablility(ListenerIndex index);

		bool SetListener(ListenerIndex index);

		void FreeListener(ListenerIndex listener);

		void RegisterWwisePlugin(const AkPluginType type, const AkUInt32 company_id,
			const AkUInt32 plugin_id, AkCreatePluginCallback create_func, AkCreateParamCallback create_param);

		ListenerIndex NextAvailableListener(void);
	private:
		AkInitSettings m_initSettings;
		AkPlatformInitSettings m_platSettings;

		std::array<bool, 8> m_listeners;

		void onAppUpdate(EVENT_HANDLER(Application));

		void init(AkInitSettings *in_pSettings, AkPlatformInitSettings *in_pPlatformSettings);

		void setGlobalListeners(void);

		void initAllListeners(void);
	} Meta(Enable, WhiteListMethods);
}

// Global JavaScript methods

Meta(Enable, ExposeJavaScript)
JSFunction(AudioPlayGlobalEvent);

Meta(Enable, ExposeJavaScript)
JSFunction(AudioStopGlobalEvent);

Meta(Enable, ExposeJavaScript)
JSFunction(AudioIsGlobalEventPlaying);
