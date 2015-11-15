/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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

#include "ListenerMasks.h"
#include "WwiseForward.h"
#include "WWiseUtils/AkFilePackageLowLevelIOBlocking.h"

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
	class AudioManager : public core::CoreSystem
	{
		CORE_SYSTEM;
	public:

		Meta( DisableNonDynamic )
		AudioManager(void) { }

		Meta(Disable)
		void OnInitialize(void) override;

		Meta(Disable)
		void OnRemove(void) override;

		void LoadBank(const std::string &bankName, AkBankID &bankID);

		void UnloadBank(const std::string &bankName);

		void RegisterObject(AkGameObjectID obj, int listener);

		void UnRegisterObject(AkGameObjectID obj);

		void PlayEvent(const std::string name, AkGameObjectID obj);

		void PauseAudio();

		void ResumeAudio();

		void SetRealTimeParameter(const std::string param, const float value, AkGameObjectID id);

		void AssignListener(AkGameObjectID obj, int listeners);

		void GetEventStrings(const AkBankID )

		void SetListener3DPosition(const AkVector orientation_forward, 
			const AkVector orientation_up, const AkVector position, const AkUInt32 listeners);

		void SetListener3DPosition(const SVec3 orientation_forward, 
			const SVec3 orientation_up, const SVec3 position, const AkUInt32 listeners);

		void SetObject3DPosition(AkGameObjectID obj, const AkSoundPosition position);

		void SetObject3DPosition(AkGameObjectID obj, const SVec3 position, const SVec3 orientation);

		void SetMultipleObject3DPosition(AkGameObjectID obj, const AkSoundPosition* positions, 
			AkUInt16 num_positions, AK::SoundEngine::MultiPositionType type);

		void SetSoundObstructionAndOcclusion(AkGameObjectID obstruction, 
			const AkUInt32 listeners, const AkReal32 obstruction_level, const AkReal32 occlusion_level);

		void SetGameState(const std::string name, const std::string state);

		void SetObjectSwitch(const std::string name, const std::string state, AkGameObjectID obj);

		void SetTrigger(const std::string name, AkGameObjectID obj);

		void RegisterWwisePlugin(const AkPluginType type, const AkUInt32 company_id, 
			const AkUInt32 plugin_id, AkCreatePluginCallback create_func, AkCreateParamCallback create_param);

		AkGameObjectID AssignSoundObjectID();

		CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

	private:
		AkInitSettings m_initSettings;
		AkPlatformInitSettings m_platSettings;

		AkGameObjectID m_nextEmptyID;
		std::vector<AkBankID> m_banksIDs;

		void onAppUpdate(EVENT_HANDLER(Application));

		void Init(AkInitSettings* in_pSettings, 
			AkPlatformInitSettings* in_pPlatformSettings, const AkOSChar* path);

	} Meta( Enable );
}
