#ifndef U3D_SOUND_ENGINE_H_
#define U3D_SOUND_ENGINE_H_

#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/MusicEngine/Common/AkMusicEngine.h>
#include <AK/SoundEngine/Common/AkModule.h>            // needed for MemManager
#include <AK/SoundEngine/Common/AkStreamMgrModule.h>

#include "AkSoundEngineExports.h"

namespace ursine
{
	namespace AK
	{
		class SoundEngine
		{
			SoundEngine();
			~SoundEngine();
			AKSOUNDENGINEDLL_API AKRESULT InitSoundEngine(AkMemSettings* MemSettings,
				AkStreamMgrSettings* StreamSettings, AkDeviceSettings* DefaultDeviceSettings,
				AkInitSettings* InitSettings, AkPlatformInitSettings* PlatformSettings,
				AkMusicSettings* MusicSettings);

			AKSOUNDENGINEDLL_API void ExitSoundEngine();

			AKSOUNDENGINEDLL_API void UpdateSoundEngine();

			AKSOUNDENGINEDLL_API void SuspendSoundEngine();

			AKSOUNDENGINEDLL_API void ResumeSoundEngine();

			AKSOUNDENGINEDLL_API AkPlayingID PostSoundEvent(AkUniqueID SE_eventID, AkGameObjectID SE_GameObjectID,
				AkUInt32 SE_EventFlags = 0, AkCallbackFunc 	SE_EventCallback = 0, void* SE_CallbackCookie = nullptr,
				AkUInt32 SE_ExternalsCount = 0, AkExternalSourceInfo* SE_ExternalSources = nullptr,
				AkPlayingID SE_PlayingID = AK_INVALID_PLAYING_ID);

			// AkOSChar is just Wwise's Generic String

			AKSOUNDENGINEDLL_API AKRESULT SetBasePath
				(
					const AkOSChar* BasePath
					);
			AKSOUNDENGINEDLL_API AKRESULT SetBankPath
				(
					const AkOSChar* BankPath
					);
			AKSOUNDENGINEDLL_API AKRESULT SetAudioSrcPath
				(
					const AkOSChar* AudioSourcePath
					);
		}
	}
}

#endif //UD3_SOUND_ENGINE_H_