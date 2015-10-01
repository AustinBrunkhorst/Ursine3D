#ifndef U3D_SOUND_ENGINE_H_
#define U3D_SOUND_ENGINE_H_

#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/MusicEngine/Common/AkMusicEngine.h>
#include <AK/SoundEngine/Common/AkModule.h>            // needed for MemManager
#include <AK/SoundEngine/Common/AkStreamMgrModule.h>

namespace ursine
{
	namespace AK
	{
		class SoundEngine
		{
			SoundEngine();
			~SoundEngine();
			AKRESULT InitSoundEngine(AkMemSettings* MemSettings,
				AkStreamMgrSettings* StreamSettings, AkDeviceSettings* DefaultDeviceSettings,
				AkInitSettings* InitSettings, AkPlatformInitSettings* PlatformSettings,
				AkMusicSettings* MusicSettings);

			void ExitSoundEngine();

			void UpdateSoundEngine();

			void SuspendSoundEngine();

			void ResumeSoundEngine();

			AkPlayingID PostSoundEvent(AkUniqueID SE_eventID, AkGameObjectID SE_GameObjectID,
				AkUInt32 SE_EventFlags = 0, AkCallbackFunc 	SE_EventCallback = 0, void* SE_CallbackCookie = nullptr,
				AkUInt32 SE_ExternalsCount = 0, AkExternalSourceInfo* SE_ExternalSources = nullptr,
				AkPlayingID SE_PlayingID = AK_INVALID_PLAYING_ID);

			// AkOSChar is just Wwise's Generic String

			AKRESULT SetBasePath(const AkOSChar* BasePath);
			AKRESULT SetBankPath(const AkOSChar* BankPath);
			AKRESULT SetAudioSrcPath(const AkOSChar* AudioSourcePath);
		};
	}
}

#endif //UD3_SOUND_ENGINE_H_