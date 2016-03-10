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
#include "Meta.h"
#include "AudioEmitterComponent.h"
#include "NativeJSFunction.h"

#include "ListenerMasks.h"
#include "WwiseForward.h"
#include "WWiseUtils/AkFilePackageLowLevelIOBlocking.h"

#include "AudioData.h"

// TODO: remove this when the shared project is removed
#if defined(URSINE_WITH_EDITOR)

#define WORLD_AUDIO_BANK_PATH URSINE_PROJECTS_DIRECTORY "Retrospect/Assets/GeneratedSoundBanks/Windows/"

#else 

#define WORLD_AUDIO_BANK_PATH "Assets/GeneratedSoundBanks/Windows/"

#endif

namespace AK
{
#ifdef WIN32
	inline void * AllocHook(size_t in_size)
	{
		return malloc(in_size);
	}
	inline void FreeHook(void * in_ptr)
	{
		free(in_ptr);
	}
	// Note: VirtualAllocHook() may be used by I/O pools of the default implementation
	// of the Stream Manager, to allow "true" unbuffered I/O (using FILE_FLAG_NO_BUFFERING
	// - refer to the Windows SDK documentation for more details). This is NOT mandatory;
	// you may implement it with a simple malloc().
	inline void * VirtualAllocHook(
		void * in_pMemAddress,
		size_t in_size,
		DWORD in_dwAllocationType,
		DWORD in_dwProtect
		)
	{
		return VirtualAlloc(in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect);
	}
	inline void VirtualFreeHook(
		void * in_pMemAddress,
		size_t in_size,
		DWORD in_dwFreeType
		)
	{
		VirtualFree(in_pMemAddress, in_size, in_dwFreeType);
	}
#endif
}

namespace ursine
{
	struct ListenerNode
	{
		ListenerNode* next;
		ListenerIndex listener;
		bool available;
	};

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
        static void SetGlobalVolume();
        static void ResetGlobalVolume();

		static void PauseAudio();

		static void StopSound(std::string name, AkGameObjectID id);

		static void ResumeAudio();

		void LoadBank(const std::string &bankName, AkBankID &bankID);
		void LoadBank(const resources::AudioData &data, AkBankID &outInit, AkBankID &outBank);

		void UnloadBank(const std::string &bankName);
		void UnloadBank(const resources::AudioData &data);

		void RegisterObject(AkGameObjectID obj, int listener);

		void UnRegisterObject(AkGameObjectID obj);

        URSINE_TODO("@Jason you need to implement this.");
		// void GetEventStrings(const AkBankID);

		ListenerIndex GetListener();

		void FreeListener(ListenerIndex listener);

		void RegisterWwisePlugin(const AkPluginType type, const AkUInt32 company_id, 
			const AkUInt32 plugin_id, AkCreatePluginCallback create_func, AkCreateParamCallback create_param);

        Meta(Disable)
		CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

	private:
		AkInitSettings m_initSettings;
		AkPlatformInitSettings m_platSettings;

		ListenerNode* m_head;

		void onAppUpdate(EVENT_HANDLER(Application));

		void PopulateList();

		void DestroyList();

		void Init(AkInitSettings* in_pSettings, 
			AkPlatformInitSettings* in_pPlatformSettings, const AkOSChar* path);

	} Meta(Enable, WhiteListMethods);
}

// Global JavaScript methods

Meta(Enable, ExposeJavaScript)
JSFunction(AudioPlayGlobalEvent);

Meta(Enable, ExposeJavaScript)
JSFunction(AudioStopGlobalEvent);

Meta(Enable, ExposeJavaScript)
JSFunction(AudioIsGlobalEventPlaying);
