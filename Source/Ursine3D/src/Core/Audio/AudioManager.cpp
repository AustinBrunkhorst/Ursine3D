#include "UrsinePrecompiled.h"

#include "AudioManager.h"

#include <Core/Audio/WWiseUtils/AkMultipleFileLocation.h>

namespace
{
	const AkOSChar* path = L"Assets\\GeneratedSoundBanks\\Windows\\";
}

namespace ursine
{
	CORE_SYSTEM_DEFINITION( AudioManager );

	void AudioManager::OnInitialize()
	{
		// Init all things from web page
		AK::SoundEngine::GetDefaultInitSettings(m_initSettings);
		AK::SoundEngine::GetDefaultPlatformInitSettings(m_platSettings);
		
		AKRESULT result = Init(&m_initSettings, &m_platSettings, path);

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
		AK::MemoryMgr::Term();
	}

	void AudioManager::onAppUpdate(void* _sender, const ursine::EventArgs* _args)
	{
		// Process bank requests, events, positions, RTPC, etc.
		AK::SoundEngine::RenderAudio();
	}

	void AudioManager::LoadBank(const std::string &bankName, AkBankID &bankID)
	{
		if (AK::SoundEngine::LoadBank(bankName.c_str(), AK_DEFAULT_POOL_ID, bankID) != AK_Success)
		{
			AKASSERT(!"Cannot Load Bank");
		}
	}

	void AudioManager::UnloadBank(const std::string &bankName)
	{
		if (AK::SoundEngine::UnloadBank(bankName.c_str(), nullptr) != AK_Success)
		{
			AKASSERT(!"Cannot Unload Bank");
		}
	}

	void AudioManager::SetInitBankID(AkBankID init)
	{
		m_initBankID = init;
	}

	void AudioManager::SetCurrentBankID(AkBankID bank)
	{
		m_curBankID = bank;
	}

	AkBankID AudioManager::GetInitBankID(void)
	{
		return m_initBankID;
	}

	AkBankID AudioManager::GetCurrentBankID(void)
	{
		return m_curBankID;
	}

	AKRESULT AudioManager::Init(AkInitSettings* in_pSettings, AkPlatformInitSettings* in_pPlatformSettings, const AkOSChar* path)
	{
		g_lowLevelIO.SetBasePath(path);

		AkMemSettings memSettings;
		memSettings.uMaxNumPools = 20;

		if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
		{
			AKASSERT(!"Could not create the memory manager.");
			return AK_Fail;
		}

		AkStreamMgrSettings stmSettings;
		AK::StreamMgr::GetDefaultSettings(stmSettings);

		// Customize the Stream Manager settings here.

		if (!AK::StreamMgr::Create(stmSettings))
		{
			AKASSERT(!"Could not create the Streaming Manager");
			return AK_Fail;
		}

		AkDeviceSettings deviceSettings;
		AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

		// Customize the streaming device settings here.

		// CAkFilePackageLowLevelIOBlocking::Init() creates a streaming device
		// in the Stream Manager, and registers itself as the File Location Resolver.
		if (g_lowLevelIO.Init(deviceSettings) != AK_Success)
		{
			AKASSERT(!"Could not create the streaming device and Low-Level I/O system");
			return AK_Fail;
		}

		// Initialize sound engine.
		if (AK::SoundEngine::Init(in_pSettings, in_pPlatformSettings) != AK_Success)
		{
			AKASSERT(!"Cannot initialize sound engine");
			return AK_Fail;
		}

		AkMusicSettings musicInit;
		AK::MusicEngine::GetDefaultInitSettings(musicInit);

		if (AK::MusicEngine::Init(&musicInit) != AK_Success)
		{
			assert(!"Could not initialize the Music Engine.");
			return AK_Fail;
		}

		return AK_Success;
	}

}
