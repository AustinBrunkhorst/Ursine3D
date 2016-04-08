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

namespace AK
{
    void *AllocHook(size_t size)
    {
        return malloc( size );
    }

    void FreeHook(void *input)
    {
        free( input );
    }

#if defined(PLATFORM_WINDOWS)

    inline void *VirtualAllocHook(
        void *in_pMemAddress,
        size_t in_size,
        DWORD in_dwAllocationType,
        DWORD in_dwProtect
    )
    {
        return VirtualAlloc( in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect );
    }

    inline void VirtualFreeHook(
        void *in_pMemAddress,
        size_t in_size,
        DWORD in_dwFreeType
    )
    {
        VirtualFree( in_pMemAddress, in_size, in_dwFreeType );
    }

#endif
}


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

        if (AK::IAkStreamMgr::Get( ))
            AK::IAkStreamMgr::Get( )->Destroy( );

        AK::MemoryMgr::Term( );
    }

    void AudioManager::OnInitialize(void)
    {
        // Init all things from web page
        AK::SoundEngine::GetDefaultInitSettings( m_initSettings );
        AK::SoundEngine::GetDefaultPlatformInitSettings( m_platSettings );

        init( &m_initSettings, &m_platSettings );

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
            data.GetInitData( ).GetData( ),
            static_cast<AkUInt32>( data.GetInitData( ).GetSize( ) ),
            outInit
        );

        UAssertCatchable( initResult == AK_Success,
            "Unable to load init bank."
        );

        auto bankResult = AK::SoundEngine::LoadBank(
            data.GetBankData( ).GetData( ),
            static_cast<AkUInt32>( data.GetBankData( ).GetSize( ) ),
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
            data.GetInitData( ).GetData( ),
            nullptr
        );

        UAssertCatchable( initResult == AK_Success,
            "Unable to unload init bank."
        );

        auto bankResult = AK::SoundEngine::UnloadBank(
            data.GetBankID( ),
            data.GetBankData( ).GetData( ),
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
            UWarning( "Wwise: Cannot Register Game Object" );
        }
    }

    void AudioManager::UnRegisterObject(AkGameObjectID obj)
    {
        if (AK::SoundEngine::UnregisterGameObj( obj ) != AK_Success)
        {
            UWarning( "Wwise: Cannot Unregister Game Object" );
        }
    }

    void AudioManager::RegisterWwisePlugin(const AkPluginType type, const AkUInt32 company_id, const AkUInt32 plugin_id, AkCreatePluginCallback create_func, AkCreateParamCallback create_param)
    {
        if (AK::SoundEngine::RegisterPlugin( type, company_id, plugin_id, create_func, create_param ))
        {
            UWarning( "Wwise: Cannot Register Plugin" );
        }
    }

    void AudioManager::init(AkInitSettings *in_pSettings, AkPlatformInitSettings *in_pPlatformSettings)
    {
        AkMemSettings memSettings;
        memSettings.uMaxNumPools = 20;

        UAssert( AK::MemoryMgr::Init( &memSettings ) == AK_Success, 
            "Wwise: Cannot Create The Memory Manager."
        );

        AK::StreamMgr::SetCurrentLanguage( AKTEXT( "English(US)" ) );

        AkStreamMgrSettings stmSettings;
        AK::StreamMgr::GetDefaultSettings( stmSettings );

        // Customize the Stream Manager settings here.

        UAssert( AK::StreamMgr::Create( stmSettings ), 
            "Wwise: Cannot Create The Streaming Manager"
        );

        AkDeviceSettings deviceSettings;
        AK::StreamMgr::GetDefaultDeviceSettings( deviceSettings );

        // Initialize sound engine.
        UAssert( AK::SoundEngine::Init( in_pSettings, in_pPlatformSettings ) == AK_Success,
            "Wwise: Cannot Initialize Sound Engine" 
        );

        AkMusicSettings musicInit;
        AK::MusicEngine::GetDefaultInitSettings( musicInit );

        UAssert( AK::MusicEngine::Init( &musicInit ) == AK_Success,
            "Wwise: Cannot Initialize The Music Engine." 
        );
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
