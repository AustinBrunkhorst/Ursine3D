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
	const AkGameObjectID StartID = 100;
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
		UAssert(AK::SoundEngine::RenderAudio() == AK_Success, "Wwise: Cannot Render Audio");
	}

	void AudioManager::LoadBank(const std::string &bankName, AkBankID &bankID)
	{
		UAssert(AK::SoundEngine::LoadBank(bankName.c_str(), 
			AK_DEFAULT_POOL_ID, bankID) == AK_Success, "Wwise: Cannot Load Bank: %s", bankName);
	}

	void AudioManager::RegisterObject(AkGameObjectID obj, int listener)
	{
		if (AK::SoundEngine::RegisterGameObj(obj, listener) != AK_Success)
		{
			UWarning("Wwise: Cannot Register Game Object");
		}

	}

	void AudioManager::UnRegisterObject(AkGameObjectID obj)
	{
		if (AK::SoundEngine::UnregisterGameObj(obj) != AK_Success)
		{
			UWarning("Wwise: Cannot Unregister Game Object");
		}
	}

	void AudioManager::PauseAudio()
	{
		UAssert(AK::SoundEngine::Suspend(false) == AK_Success, "Wwise: Cannot Suspend Sound Engine");
	}

	void AudioManager::ResumeAudio()
	{
		UAssert( AK::SoundEngine::WakeupFromSuspend( ) == AK_Success,
			"Wwise: Cannot Wake Up Sound Engine From Suspend" );
	}

	void AudioManager::SetRealTimeParameter(const std::string param, const float value, AkGameObjectID id)
	{
		if (AK::SoundEngine::SetRTPCValue(param.c_str(), (AkRtpcValue)value, id) != AK_Success)
		{
			UWarning("Wwise: Cannot Set RTPC value: %s", param.c_str());
		}
	}

	void AudioManager::AssignListener(AkGameObjectID obj, int listeners)
	{
		if (listeners == LISTENER_NONE)
		{
			UWarning("Wwise: Cannot Set Zero Active Listeners");
			return;
		}

		if (AK::SoundEngine::SetActiveListeners(obj, listeners) != AK_Success)
		{
			UWarning("Wwise: Cannot Set Active Listeners");
		}
	}

	void AudioManager::SetListener3DPosition(const AkVector orientation_forward, 
		const AkVector orientation_up, const AkVector position, const AkUInt32 listeners)
	{
		if (position.X == 0 &&
			position.Y == 0 &&
			position.Z == 0)
			return;

		AkListenerPosition listenerPosition;
		// up and forward have to be orthogonal and normalized
		listenerPosition.OrientationTop = orientation_up;
		listenerPosition.OrientationFront = orientation_forward;
		listenerPosition.Position = position;

		if (AK::SoundEngine::SetListenerPosition(listenerPosition, listeners) != AK_Success)
		{
			UWarning("Wwise: Cannot Set Listener Postion");
		}
	}

	void AudioManager::SetListener3DPosition(const SVec3 orientation_forward, 
		const SVec3 orientation_up, const SVec3 position, const AkUInt32 listeners)
	{
		if (position.X( ) == 0 &&
			position.Y( ) == 0 &&
			position.Z( ) == 0)
			return;

		AkListenerPosition listenerPosition;
		// up and forward have to be orthogonal and normalized
		listenerPosition.OrientationTop = orientation_up.ToWwise();
		listenerPosition.OrientationFront = orientation_forward.ToWwise();
		listenerPosition.Position = position.ToWwise();

		if (AK::SoundEngine::SetListenerPosition(listenerPosition, listeners) != AK_Success)
		{
			UWarning("Wwise: Cannot Set Listener Postion");
		}
	}

	void AudioManager::SetObject3DPosition(AkGameObjectID obj, const AkSoundPosition position)
	{
		if (AK::SoundEngine::SetPosition(obj, position) != AK_Success)
		{
			UWarning("Wwise: Cannot Set Object 3D Position");
		}
	}

	void AudioManager::SetMultipleObject3DPosition(AkGameObjectID obj, const AkSoundPosition* positions, 
		AkUInt16 num_positions, AK::SoundEngine::MultiPositionType type)
	{
		// MultiPositionType_MultiSources 	
        // Simulate multiple sources in one sound playing, adding volumes. 
		// For instance, all the torches on your level emitting using only one sound.
        
		// MultiPositionType_MultiDirections
		// Simulate one sound coming from multiple directions. Useful for repositioning 
		// sounds based on wall openings or to simulate areas like forest or rivers

		if (SetMultiplePositions(obj, positions, num_positions, type) != AK_Success)
		{
			UWarning("Wwise: Cannot Set Mutiple Object 3D Position");
		}
	}

	void AudioManager::SetSoundObstructionAndOcclusion(AkGameObjectID obstruction, 
		const AkUInt32 listeners, const AkReal32 obstruction_level, const AkReal32 occlusion_level)
	{
		if (AK::SoundEngine::SetObjectObstructionAndOcclusion(obstruction, listeners, 
			obstruction_level, occlusion_level) != AK_Success)
		{
			UWarning("Wwise: Cannot Set Obstruction and Occlusion");
		}
	}

	void AudioManager::SetGameState(const std::string name, const std::string state)
	{
		// global state change
		if (AK::SoundEngine::SetState(name.c_str(), state.c_str()))
		{
			UWarning("Wwise: Cannot Set State");
		}
	}

	void AudioManager::SetObjectSwitch(const std::string name, const std::string state, AkGameObjectID obj)
	{
		// specific states for each object
		if (AK::SoundEngine::SetSwitch(name.c_str(), state.c_str(), obj))
		{
			UWarning("Wwise: Cannot Set Switch");
		}
	}

	void AudioManager::SetTrigger(const std::string name, AkGameObjectID obj)
	{
		if (AK::SoundEngine::PostTrigger(name.c_str(), obj))
		{
			UWarning("Wwise: Cannot Post Trigger");
		}
	}

	void AudioManager::RegisterWwisePlugin(const AkPluginType type, const AkUInt32 company_id, const AkUInt32 plugin_id, AkCreatePluginCallback create_func, AkCreateParamCallback create_param)
	{
		if (AK::SoundEngine::RegisterPlugin(type, company_id, plugin_id, create_func, create_param))
		{
			UWarning("Wwise: Cannot Register Plugin");
		}
	}

	void AudioManager::SetObject3DPosition(AkGameObjectID obj, const SVec3 position, const SVec3 orientation)
	{
		AkSoundPosition pos;
		pos.Position = position.ToWwise();
		pos.Orientation = orientation.ToWwise();
		if (AK::SoundEngine::SetPosition(obj, pos) != AK_Success)
		{
			UWarning("Wwise: Cannot Set Object 3D Position");
		}
	}

	void AudioManager::PlayEvent(const std::string name, AkGameObjectID obj)
	{
		if (AK::SoundEngine::PostEvent(name.c_str(), obj) != AK_Success)
		{
			UWarning("Wwise: Cannot Post Event: %s", name.c_str());
		}
	}

	void AudioManager::UnloadBank(const std::string &bankName)
	{
		UAssert(AK::SoundEngine::UnloadBank(bankName.c_str(), 
			nullptr) == AK_Success, "Wwise: Cannot Unload Bank: %s", bankName.c_str());
	}

	void AudioManager::Init(AkInitSettings *in_pSettings, AkPlatformInitSettings *in_pPlatformSettings, const AkOSChar *path)
	{
		g_lowLevelIO.SetBasePath(path);

		AkMemSettings memSettings;
		memSettings.uMaxNumPools = 20;

		UAssert(AK::MemoryMgr::Init(&memSettings) == AK_Success, "Wwise: Cannot Create The Memory Manager.");

		AkStreamMgrSettings stmSettings;
		AK::StreamMgr::GetDefaultSettings(stmSettings);

		// Customize the Stream Manager settings here.

		UAssert(AK::StreamMgr::Create(stmSettings), "Wwise: Cannot Create The Streaming Manager");

		AkDeviceSettings deviceSettings;
		AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

		// Customize the streaming device settings here.

		// CAkFilePackageLowLevelIOBlocking::Init() creates a streaming device
		// in the Stream Manager, and registers itself as the File Location Resolver.

		UAssert(g_lowLevelIO.Init(deviceSettings) == AK_Success, 
			"Wwise: Cannot Create The Streaming Device And Low-Level I/O System");

		// Initialize sound engine.
		UAssert(AK::SoundEngine::Init(in_pSettings, in_pPlatformSettings) == AK_Success, 
			"Wwise: Cannot Initialize Sound Engine");

		AkMusicSettings musicInit;
		AK::MusicEngine::GetDefaultInitSettings(musicInit);

		UAssert(AK::MusicEngine::Init(&musicInit) == AK_Success, 
			"Wwise: Cannot Initialize The Music Engine.");
	}
}
