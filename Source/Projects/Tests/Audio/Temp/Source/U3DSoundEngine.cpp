#include "U3DSoundEngine.h"
#include <AK/Tools/Common/AkAssert.h>            // Wwise generics asserts
#include <malloc.h>                   ///Change

#include <AK/SoundEngine/Common/AkMemoryMgr.h>   // Wwise SEngine MemMan
#include <AK/SoundEngine/Common/IAkStreamMgr.h>  // Stream Manager Interface
//#include "AkDefaultIOHookBlocking.h"  ///Change
#include <AK\AkDefaultIOHookBlocking.h>

// AKCommunication is used to enable communication between Wwise and 
// the game so the Sound Dev can perform in-game mixing and profiling
// BUT, it can't be used when in release, as per most profilers/debuggers
#ifndef AK_OPTIMIZED
#include <AK/Comm/AkCommunication.h>
#endif

#include <AK/Plugin/AllPluginsRegistrationHelpers.h> // Handles all plugins at once


// Default memory manager settings.
static unsigned int COMMUNICATION_POOL_SIZE = 256 * 1024;
static unsigned int COMMUNICATION_POOL_BLOCKS = 48;

namespace ursine
{
	namespace AK
	{
		SoundEngine::SoundEngine()
		{

		}
		SoundEngine::~SoundEngine()
		{

		}

		// This is straight from the website
		// This is allocating memory for the SEngine MemMan
		// They use malloc/free, not sure if it works with new/delete

		// Allocating Memory for the SEngine MemMan
		void * AllocHook(size_t mem_size)
		{
			return malloc(mem_size);
		}

		// Freeing Memory
		void FreeHook(void * mem_ptr)
		{
			free(mem_ptr);
		}

		// Virtual Allocation
		void * VirtualAllocHook(void* mem_address, size_t mem_size, DWORD dw_allocation_type, DWORD dw_protect)
		{
			return VirtualAlloc(mem_address, mem_size, dw_allocation_type, dw_protect);
		}

		// Virtual Free
		void VirtualFreeHook(void * mem_address, size_t mem_size, DWORD dw_free_type)
		{
			VirtualFree(mem_address, mem_size, dw_free_type);
		}

		/////////////////// Member Functions ///////////////////////////////////

		AKRESULT SoundEngine::InitSoundEngine(AkMemSettings* MemSettings,
			AkStreamMgrSettings* StreamSettings, AkDeviceSettings* DefaultDeviceSettings,
			AkInitSettings* InitSettings, AkPlatformInitSettings* PlatformSettings,
			AkMusicSettings* MusicSettings)
		{
			// check required arguments.
			if (!MemSettings || !StreamSettings || !DefaultDeviceSettings)
			{
				AKASSERT(!"Invalid arguments");
				return AK_InvalidParameter;
			}

			// create and initialize an instance of our memory manager.
			if (MemoryMgr::Init(MemSettings) != AK_Success)
			{
				AKASSERT(!"Could not create the memory manager.");
				return AK_Fail;
			}

			// create and initialize an instance of the default stream manager.
			if (!StreamMgr::Create(*StreamSettings))
			{
				AKASSERT(!"Could not create the Stream Manager");
				return AK_Fail;
			}

			// create an IO device.
			if (m_lowLevelIO.Init(*DefaultDeviceSettings) != AK_Success)
			{
				AKASSERT(!"Cannot create streaming I/O device");
				return AK_Fail;
			}

			// initialize sound engine.
			if (SoundEngine::Init(InitSettings, PlatformSettings) != AK_Success)
			{
				AKASSERT(!"Cannot initialize sound engine");
				return AK_Fail;
			}

			// initialize music engine.
			if (MusicEngine::Init(MusicSettings) != AK_Success)
			{
				AKASSERT(!"Cannot initialize music engine");
				return AK_Fail;
			}

#ifndef AK_OPTIMIZED
			// initialize communication.
			AkCommSettings CommSettings;
			AK::Comm::GetDefaultInitSettings(CommSettings);
			if (AK::Comm::Init(CommSettings) != AK_Success)
			{
				AKASSERT(!"Cannot initialize communication");
				return AK_Fail;
			}
#endif // AK_OPTIMIZED

			// Register plugins
			/// Note: This a convenience method for rapid prototyping. 
			/// To reduce executable code size register/link only the plug-ins required by your game 
			if (AK::SoundEngine::RegisterAllPlugins() != AK_Success)
			{
				AKASSERT(!"Error while registering plug-ins");
				return AK_Fail;
			}

			return AK_Success;
		}

		void SoundEngine::ExitSoundEngine()
		{
#ifndef AK_OPTIMIZED
			Comm::Term();
#endif // AK_OPTIMIZED

			MusicEngine::Term();

			SoundEngine::Term();

			m_lowLevelIO.Term();
			if (IAkStreamMgr::Get())
				IAkStreamMgr::Get()->Destroy();

			MemoryMgr::Term();
		}

		// sound engine processes all pending bank load/unload requests, events, 
		// position changes, states, switches, RTPCs, etc. If communications are 
		// enabled, they are also processed within RenderAudio().
		void SoundEngine::UpdateSoundEngine()
		{
			SoundEngine::RenderAudio();
		}

		// this can be used for minimizing the game or whatever
		void SoundEngine::SuspendSoundEngine()
		{
			AK::SoundEngine::Suspend(false);
		}

		void SoundEngine::ResumeSoundEngine()
		{
			AK::SoundEngine::WakeupFromSuspend();
		}

		AkPlayingID SoundEngine::PostSoundEvent(AkUniqueID SE_eventID, AkGameObjectID SE_GameObjectID,
			AkUInt32 SE_EventFlags, AkCallbackFunc 	SE_EventCallback, void* SE_CallbackCookie, AkUInt32 SE_ExternalsCount,
			AkExternalSourceInfo* SE_ExternalSources, AkPlayingID SE_PlayingID)
		{
			// this calls PostEvent with all preset options
			// if it returns AK_INVALID_PLAYING_ID then something didn't work
			AkPlayingID playingID = AK::SoundEngine::PostEvent(SE_eventID,
				SE_GameObjectID, SE_EventFlags, SE_EventCallback, SE_CallbackCookie,
				SE_ExternalsCount, SE_ExternalSources, SE_PlayingID);

			if (playingID == AK_INVALID_PLAYING_ID)
			{
				// declare problem, ask Austin
				AKASSERT(!"Cannot Post Event");
			}

			return playingID;
		}

		AKRESULT SoundEngine::SetBasePath(const AkOSChar* BasePath)
		{
			return m_lowLevelIO.SetBasePath(BasePath);
		}

		AKRESULT SoundEngine::SetBankPath(const AkOSChar* BankPath)
		{
			return m_lowLevelIO.SetBankPath(BankPath);
		}

		AKRESULT SoundEngine::SetAudioSrcPath(const AkOSChar* AudioSourcePath)
		{
			return m_lowLevelIO.SetAudioSrcPath(AudioSourcePath);
		}
	}
}