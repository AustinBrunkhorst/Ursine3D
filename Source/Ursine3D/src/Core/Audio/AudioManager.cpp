/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AudioManager.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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

#include "AudioID.h"
#include "SoundFrameClient.h"

namespace
{
    const AkGameObjectID StartID = 100;

    const std::string kInitBank = "INIT";
    const std::string kMainBank = "MAIN";

    const std::string kSecondaryBus = "MASTER_SECONDARY_BUS";

    AkBankID BankID = AK_INVALID_BANK_ID;
    AkBankID MainID = AK_INVALID_BANK_ID;

    std::unordered_map<std::string, AkPlayingID> &getCreatedGlobalEvents(void)
    {
        static std::unordered_map<std::string, AkPlayingID> globalEvents;

        return globalEvents;
    }
}

namespace ursine
{
    CORE_SYSTEM_DEFINITION( AudioManager );

    AudioManager::~AudioManager(void)
    {
        AK::SoundEngine::UnregisterAllGameObj( );

        AK::MusicEngine::Term( );

        AK::SoundEngine::Term( );

        g_lowLevelIO.Term( );

        if (AK::IAkStreamMgr::Get( ))
            AK::IAkStreamMgr::Get( )->Destroy( );

        AK::MemoryMgr::Term( );
    }

    void AudioManager::OnInitialize(void)
    {
        // Init all things from web page
        AK::SoundEngine::GetDefaultInitSettings( m_initSettings );
        AK::SoundEngine::GetDefaultPlatformInitSettings( m_platSettings );

        Init( &m_initSettings, &m_platSettings, WIDEN( WORLD_AUDIO_BANK_PATH ) );

        // Init all Listeners
		InitAllListeners( );

		// Create the global listener
		SetGlobalListener( );

        // Subscribe to update
        Application::Instance->Connect( APP_UPDATE, this, &AudioManager::onAppUpdate );
    }

    void AudioManager::PauseAudio(void)
    {
        AK::SoundEngine::Suspend( false );
    }

    void AudioManager::StopSound(std::string name, AkGameObjectID id)
    {
        auto result = ExecuteActionOnEvent(
            name.c_str( ),
            AK::SoundEngine::AkActionOnEventType_Stop,
            id
        );

        UWarningIf( result != AK_Success, 
            "Wwise: Could Not Stop The Music!! Or this event." 
        );
    }

    void AudioManager::ResumeAudio(void)
    {
        AK::SoundEngine::WakeupFromSuspend( );
    }

    void AudioManager::PlayEvent(const std::string name, AkGameObjectID obj)
    {
        if (AK::SoundEngine::PostEvent( name.c_str( ), obj ) == AK_INVALID_PLAYING_ID)
        {
            UWarning( "Wwise: Cannot Post Event: %s", name.c_str( ) );
        }
    }

    void AudioManager::PlayGlobalEvent(const std::string &name)
    {
        auto result = AK::SoundEngine::PostEvent(
            name.c_str( ),
            AUDIO_GLOBAL_OBJECT_ID,
            AK_EnableGetSourcePlayPosition
        );

        if (result == AK_INVALID_PLAYING_ID)
        {
            UWarning( "Wwise: Cannot Post Global Event: %s", name.c_str( ) );
        }
        else
        {
            auto &globalEvents = getCreatedGlobalEvents( );

            globalEvents[ name ] = result;
        }
    }

    void AudioManager::StopGlobalEvent(const std::string &name)
    {
        auto &globalEvents = getCreatedGlobalEvents( );

        auto result = ExecuteActionOnEvent(
            name.c_str( ),
            AK::SoundEngine::AkActionOnEventType_Stop,
            AUDIO_GLOBAL_OBJECT_ID
        );

        if (result == AK_Success)
        {
            globalEvents.erase( name );
        }
        else
        {
            UWarning( "Wwise: Could Not Stop The Music!! Or this event." );
        }
    }

    bool AudioManager::IsGlobalEventPlaying(const std::string &name)
    {
        auto &globalEvents = getCreatedGlobalEvents( );

        auto search = globalEvents.find( name );

        // doesn't exist, so it can't be playing
        if (search == globalEvents.end( ))
            return false;

        AkTimeMs position;

        auto result = AK::SoundEngine::GetSourcePlayPosition(
            search->second,
            &position
        );

        return result == AK_Success;
    }

    void AudioManager::OnRemove(void)
    {
        Application::Instance->Disconnect( APP_UPDATE, this, &AudioManager::onAppUpdate );
    }

    void AudioManager::onAppUpdate(void *_sender, const ursine::EventArgs *_args)
    {
        // Process bank requests, events, positions, RTPC, etc.
        UAssert( AK::SoundEngine::RenderAudio( ) == AK_Success, "Wwise: Cannot Render Audio" );
    }

    void AudioManager::LoadBank(const resources::AudioData &data, AkBankID &outInit, AkBankID &outBank)
    {
        auto initResult = AK::SoundEngine::LoadBank( 
            data.GetInitBytes( ), 
            static_cast<AkUInt32>( data.GetInitByteSize( ) ), 
            outInit
        );

        UAssertCatchable( initResult == AK_Success,
            "Unable to load init bank."    
        );

        auto bankResult = AK::SoundEngine::LoadBank( 
            data.GetBankBytes( ), 
            static_cast<AkUInt32>( data.GetBankSize( ) ), 
            outBank
        );

        UAssertCatchable( bankResult == AK_Success,
            "Unable to load bank."    
        );
    }

    void AudioManager::UnloadBank(const resources::AudioData &data)
    {
        auto initResult = AK::SoundEngine::UnloadBank(
            data.GetInitID( ),
            data.GetInitBytes( ),
            nullptr
        );

        UAssertCatchable( initResult == AK_Success,
            "Unable to unload init bank."    
        );

        auto bankResult = AK::SoundEngine::UnloadBank(
            data.GetBankID( ),
            data.GetBankBytes( ),
            nullptr
        );

        UAssertCatchable( bankResult == AK_Success,
            "Unable to unload bank."
        );
    }

    void AudioManager::RegisterObject(AkGameObjectID obj, int listener)
    {
        if (AK::SoundEngine::RegisterGameObj( obj, listener ) != AK_Success)
        {
            UWarning("Wwise: Cannot Register Game Object");
        }
    }

	ListenerMask AudioManager::IndexToMask(ListenerIndex index)
	{
		return static_cast<ListenerMask>( index );
	}

	const int AudioManager::IndexToInt(ListenerIndex index)
	{
		switch (index)
		{
		case ListenerIndex::None:
		{
			return -1;
		}
		case ListenerIndex::L1:
		{
			return 0;
		}
		case ListenerIndex::L2:
		{
			return 1;
		}
		case ListenerIndex::L3:
		{
			return 2;
		}
		case ListenerIndex::L4:
		{
			return 3;
		}
		case ListenerIndex::L5:
		{
			return 4;
		}
		case ListenerIndex::L6:
		{
			return 5;
		}
		case ListenerIndex::L7:
		{
			return 6;
		}
		case ListenerIndex::L8:
		{
			return 7;
		}
		}
		return -1;
	}

	ListenerIndex AudioManager::IntToIndex(int val)
	{
		return static_cast<ListenerIndex>( val );
	}

	ListenerIndex AudioManager::MaskToIndex(ListenerMask mask)
	{
		return static_cast<ListenerIndex>( mask );
	}

	bool AudioManager::GetListenerAvailablility(ListenerIndex index)
    {
		return m_listeners[ IndexToInt( index ) ];
    }

	bool AudioManager::SetListener(ListenerIndex index)
    {
	    if (m_listeners[ IndexToInt( index ) ])
	    {
			return false;
	    }
		else
		{
			m_listeners[ IndexToInt( index ) ] = true;
			return true;
		}
    }

	void AudioManager::FreeListener(ListenerIndex listener)
    {
		if (listener == ListenerIndex::None)
		{
			UWarning("Wwise: Cannot Index with Listener::None");
		}
	    else if (m_listeners[ IndexToInt( listener ) ])
	    {
			m_listeners[ IndexToInt( listener ) ] = false;
	    }
    }

	void AudioManager::UnRegisterObject(AkGameObjectID obj)
    {
        if (AK::SoundEngine::UnregisterGameObj( obj ) != AK_Success)
        {
            UWarning("Wwise: Cannot Unregister Game Object");
        }
    }

    void AudioManager::RegisterWwisePlugin(const AkPluginType type, const AkUInt32 company_id, const AkUInt32 plugin_id, AkCreatePluginCallback create_func, AkCreateParamCallback create_param)
    {
        if (AK::SoundEngine::RegisterPlugin( type, company_id, plugin_id, create_func, create_param ))
        {
            UWarning("Wwise: Cannot Register Plugin");
        }
    }

	void AudioManager::SetGlobalListener()
    {
		// create an object that emits to all listeners
		RegisterObject(AUDIO_GLOBAL_OBJECT_ID, 0x000000FF);

		// set position of the listener and the emitter to (0,0,0)
		AkSoundPosition pos;
		AkListenerPosition lpos;
		pos.Position.X = pos.Position.Y = pos.Position.Z = 0.0f;
		lpos.Position = pos.Position;
		pos.Orientation.X = pos.Orientation.Y = 0.0f;
		pos.Orientation.Z = -1.0f;
		lpos.OrientationFront = pos.Orientation;
		lpos.OrientationTop.X = lpos.OrientationTop.Z = 0.0f;
		lpos.OrientationTop.Y = 1.0f;

		if (AK::SoundEngine::SetPosition( AUDIO_GLOBAL_OBJECT_ID, pos ) != AK_Success)
		{
			UWarning( "Wwise: Cannot Set Object 3D Position" );
		}

		if (AK::SoundEngine::SetListenerPosition( lpos, 0x00000001 ) != AK_Success)
		{
			UWarning( "Wwise: Cannot Set Listener Postion" );
		}
    }

	void AudioManager::InitAllListeners()
    {
		m_listeners.fill( false );
		m_listeners[ 0 ] = true;
    }

	void AudioManager::Init(AkInitSettings *in_pSettings, AkPlatformInitSettings *in_pPlatformSettings, const AkOSChar *path)
    {
        g_lowLevelIO.SetBasePath( path );

        AkMemSettings memSettings;
        memSettings.uMaxNumPools = 20;

        UAssert(AK::MemoryMgr::Init(&memSettings) == AK_Success, "Wwise: Cannot Create The Memory Manager.");

        AK::StreamMgr::SetCurrentLanguage( AKTEXT("English(US)") );

        AkStreamMgrSettings stmSettings;
        AK::StreamMgr::GetDefaultSettings( stmSettings );

        // Customize the Stream Manager settings here.

        UAssert(AK::StreamMgr::Create(stmSettings), "Wwise: Cannot Create The Streaming Manager");

        AkDeviceSettings deviceSettings;
        AK::StreamMgr::GetDefaultDeviceSettings( deviceSettings );

        // Customize the streaming device settings here.

        // CAkFilePackageLowLevelIOBlocking::Init() creates a streaming device
        // in the Stream Manager, and registers itself as the File Location Resolver.

        UAssert(g_lowLevelIO.Init(deviceSettings) == AK_Success,
            "Wwise: Cannot Create The Streaming Device And Low-Level I/O System");

        // Initialize sound engine.
        UAssert(AK::SoundEngine::Init(in_pSettings, in_pPlatformSettings) == AK_Success,
            "Wwise: Cannot Initialize Sound Engine");

        AkMusicSettings musicInit;
        AK::MusicEngine::GetDefaultInitSettings( musicInit );

        UAssert(AK::MusicEngine::Init(&musicInit) == AK_Success,
            "Wwise: Cannot Initialize The Music Engine.");

        //LoadBank( kInitBank, BankID );
        //LoadBank( kMainBank, MainID );
    }
}

JSFunction(AudioPlayGlobalEvent)
{
    if (arguments.size( ) != 1)
        JSThrow( "Invalid arguments.", nullptr );

    ursine::AudioManager::PlayGlobalEvent(
        arguments[ 0 ]->GetStringValue( )
    );

    return CefV8Value::CreateBool( true );
}

JSFunction(AudioStopGlobalEvent)
{
    if (arguments.size( ) != 1)
        JSThrow(" Invalid arguments.", nullptr );

    ursine::AudioManager::StopGlobalEvent(
        arguments[ 0 ]->GetStringValue( )
    );

    return CefV8Value::CreateBool( true );
}

JSFunction(AudioIsGlobalEventPlaying)
{
    if (arguments.size( ) != 1)
        JSThrow(" Invalid arguments.", nullptr );

    auto result = ursine::AudioManager::IsGlobalEventPlaying(
        arguments[ 0 ]->GetStringValue( )
    );

    return CefV8Value::CreateBool( result );
}
