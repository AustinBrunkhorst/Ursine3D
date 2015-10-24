#include "UrsinePrecompiled.h"

#include "AudioManager.h"

#include <Core/Audio/WWiseUtils/AkMultipleFileLocation.h>

#include "Math/SIMD/SVec3.h"

#include <AK/SoundEngine/Common/AkSoundEngine.h>

#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/AkModule.h>

#include <AK/SoundEngine/Common/IAkStreamMgr.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>
#include <WWiseUtils/AkFilePackageLowLevelIOBlocking.h>

#include <AK/MusicEngine/Common/AkMusicEngine.h>

namespace
{
	const AkOSChar *path = L"Assets\\GeneratedSoundBanks\\Windows\\";
}

namespace ursine
{
	CORE_SYSTEM_DEFINITION( AudioManager ) ;

	void AudioManager::OnInitialize()
	{
		// Init all things from web page
		AK::SoundEngine::GetDefaultInitSettings(m_initSettings);
		AK::SoundEngine::GetDefaultPlatformInitSettings(m_platSettings);

		Init(&m_initSettings, &m_platSettings, path);

		// Client is responsible for loading/unloading banks, starting and ending
		// with the Init Bank

		// make app
		auto *app = Application::Instance;

		// Subscribe to update
		app->Connect(APP_UPDATE, this, &AudioManager::onAppUpdate);
	}

	void AudioManager::OnRemove()
	{
		AK::MusicEngine::Term();

		AK::SoundEngine::Term();

		g_lowLevelIO.Term();

		if (AK::IAkStreamMgr::Get())
			AK::IAkStreamMgr::Get()->Destroy();

		AK::MemoryMgr::Term();
	}

	void AudioManager::onAppUpdate(void *_sender, const ursine::EventArgs *_args)
	{
		// Process bank requests, events, positions, RTPC, etc.
		UAssert(AK::SoundEngine::RenderAudio() == AK_Success, "Cannot Render Audio");
	}

	void AudioManager::LoadBank(const std::string &bankName, AkBankID &bankID)
	{
		UAssert(AK::SoundEngine::LoadBank(bankName.c_str(), 
			AK_DEFAULT_POOL_ID, bankID) == AK_Success, "Cannot Load Bank: %s", bankName);
	}

	void AudioManager::RegisterObject(AkGameObjectID obj, int listener)
	{
		if (AK::SoundEngine::RegisterGameObj(obj, listener) != AK_Success)
		{
			UWarning("Cannot Set RTPC value");
		}

	}

	void AudioManager::PauseAudio()
	{
		UAssert(AK::SoundEngine::Suspend(false) == AK_Success, "Cannot Suspend Sound Engine");
	}

	void AudioManager::ResumeAudio()
	{
		UAssert( AK::SoundEngine::WakeupFromSuspend( ) == AK_Success,
			"Cannot wake up Sound Engine from Suspend" );
	}

	void AudioManager::SetRealTimeParameter(const std::string param, const float value, AkGameObjectID id)
	{
		if (AK::SoundEngine::SetRTPCValue(param.c_str(), (AkRtpcValue)value, id) != AK_Success)
		{
			UWarning("Cannot Set RTPC value: %s", param.c_str());
		}
	}

	void AudioManager::AssignListener(AkGameObjectID obj, int listeners)
	{
		if (listeners == LISTENER_NONE)
		{
			UWarning("Cannot Set zero Active Listeners");
			return;
		}

		if (AK::SoundEngine::SetActiveListeners(obj, listeners) != AK_Success)
		{
			UWarning("Cannot Set Active Listeners");
		}
	}

	void AudioManager::SetListenerPosition(const AkVector orientation_forward, const AkVector orientation_up, const AkVector position)
	{
		if (position.X == 0 &&
			position.Y == 0 &&
			position.Z == 0)
			return;

		AkListenerPosition listenerPosition;
		listenerPosition.OrientationTop = orientation_up;
		listenerPosition.OrientationFront = orientation_forward;
		listenerPosition.Position = position;

		if (AK::SoundEngine::SetListenerPosition(listenerPosition) != AK_Success)
		{
			UWarning("Cannot Set Listener Postion");
		}
	}

	void AudioManager::SetListenerPosition(const SVec3 orientation_forward, const SVec3 orientation_up, const SVec3 position)
	{
		if (position.X( ) == 0 &&
			position.Y( ) == 0 &&
			position.Z( ) == 0)
			return;

		AkListenerPosition listenerPosition;
		listenerPosition.OrientationTop = orientation_up.ToWwise();
		listenerPosition.OrientationFront = orientation_forward.ToWwise();
		listenerPosition.Position = position.ToWwise();

		if (AK::SoundEngine::SetListenerPosition(listenerPosition) != AK_Success)
		{
			UWarning("Cannot Set Listener Postion");
		}
	}

	void AudioManager::SetObject3DPosition(AkGameObjectID obj, const AkSoundPosition position)
	{
		if (AK::SoundEngine::SetPosition(obj, position) != AK_Success)
		{
			UWarning("Cannot Set Object 3D Position");
		}
	}

	void AudioManager::SetObject3DPosition(AkGameObjectID obj, const SVec3 position, const SVec3 orientation)
	{
		AkSoundPosition pos;
		pos.Position = position.ToWwise();
		pos.Orientation = orientation.ToWwise();
		if (AK::SoundEngine::SetPosition(obj, pos) != AK_Success)
		{
			UWarning("Cannot Set Object 3D Position");
		}
	}

	void AudioManager::PlayEvent(const std::string name, AkGameObjectID obj)
	{
		if (AK::SoundEngine::PostEvent(name.c_str(), obj) != AK_Success)
		{
			UWarning("Cannot Post Event: %s", name.c_str());
		}
	}

	void AudioManager::UnloadBank(const std::string &bankName)
	{
		UAssert(AK::SoundEngine::UnloadBank(bankName.c_str(), 
			nullptr) == AK_Success, "Cannot Unload Bank: %s", bankName.c_str());
	}

	void AudioManager::Init(AkInitSettings *in_pSettings, AkPlatformInitSettings *in_pPlatformSettings, const AkOSChar *path)
	{
		g_lowLevelIO.SetBasePath(path);

		AkMemSettings memSettings;
		memSettings.uMaxNumPools = 20;

		UAssert(AK::MemoryMgr::Init(&memSettings) == AK_Success, "Could not create the memory manager.");

		AkStreamMgrSettings stmSettings;
		AK::StreamMgr::GetDefaultSettings(stmSettings);

		// Customize the Stream Manager settings here.

		UAssert(AK::StreamMgr::Create(stmSettings), "Could not create the Streaming Manager");

		AkDeviceSettings deviceSettings;
		AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

		// Customize the streaming device settings here.

		// CAkFilePackageLowLevelIOBlocking::Init() creates a streaming device
		// in the Stream Manager, and registers itself as the File Location Resolver.

		UAssert(g_lowLevelIO.Init(deviceSettings) == AK_Success, 
			"Could not create the streaming device and Low-Level I/O system");

		// Initialize sound engine.
		UAssert(AK::SoundEngine::Init(in_pSettings, in_pPlatformSettings) == AK_Success, 
			"Cannot initialize sound engine");

		AkMusicSettings musicInit;
		AK::MusicEngine::GetDefaultInitSettings(musicInit);

		UAssert(AK::MusicEngine::Init(&musicInit) == AK_Success, 
			"Could not initialize the Music Engine.");
	}
}
