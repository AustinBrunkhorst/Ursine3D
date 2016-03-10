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

        //UnloadBank( kMainBank );
        //UnloadBank( kInitBank );

        DestroyList( );

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

        PopulateList( );

        RegisterObject( AUDIO_GLOBAL_OBJECT_ID, 0x000000FF );

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
            UWarning("Wwise: Cannot Set Object 3D Position");
        }

        if (AK::SoundEngine::SetListenerPosition( lpos, 0x00000001 ) != AK_Success)
        {
            UWarning("Wwise: Cannot Set Listener Postion");
        }

        // Client is responsible for loading/unloading banks, starting and ending
        // with the Init Bank

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

    void AudioManager::DestroyList(void)
    {
        for (int i = 0; i < 8; ++i)
        {
            auto temp = m_head;
            m_head = m_head->next;
            delete temp;
        }
    }

    void AudioManager::PopulateList(void)
    {
        ListenerIndex list[8] = {
            ListenerIndex::One,
            ListenerIndex::Two,
            ListenerIndex::Three,
            ListenerIndex::Four,
            ListenerIndex::Five,
            ListenerIndex::Six,
            ListenerIndex::Seven,
            ListenerIndex::Eight
        };

        auto head = new ListenerNode;
        head->listener = list[ 0 ];
        head->available = true;
        m_head = head;
        auto temp = head;
        for (int i = 1; i < 8; ++i)
        {
            auto node = new ListenerNode;
            node->listener = list[ i ];
            node->available = true;
            temp->next = node;
            temp = temp->next;
        }
        temp->next = nullptr;
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

    void AudioManager::SetGlobalVolume(void)
    {
       //AK::SoundEngine::SetRTPCValue(  );
    }

    void AudioManager::ResetGlobalVolume(void)
    {
        //AK::SoundEngine::ResetRTPCValue();
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
        UAssert(AK::SoundEngine::RenderAudio() == AK_Success, "Wwise: Cannot Render Audio");
    }

    void AudioManager::LoadBank(const std::string &bankName, AkBankID &bankID)
    {
        UAssert(AK::SoundEngine::LoadBank(bankName.c_str(),
            AK_DEFAULT_POOL_ID, bankID) == AK_Success, "Wwise: Cannot Load Bank: %s", bankName);
    }

    void AudioManager::LoadBank(const resources::AudioData &data, AkBankID &outInit, AkBankID &outBank)
    {
        auto &initData = data.GetInitData( );
        auto &bankData = data.GetBankData( );

        auto initResult = AK::SoundEngine::LoadBank( 
            initData.GetData( ), 
            static_cast<AkUInt32>( initData.GetSize( ) ), 
            outInit
        );

        UAssertCatchable( initResult == AK_Success,
            "Unable to load init bank."    
        );

        auto bankResult = AK::SoundEngine::LoadBank( 
            bankData.GetData( ),
            static_cast<AkUInt32>( bankData.GetSize( ) ),
            outBank
        );

        UAssertCatchable( bankResult == AK_Success,
            "Unable to load bank."    
        );
    }

    void AudioManager::UnloadBank(const std::string &bankName)
    {
        URSINE_TODO( "@Jason" );
       /* UAssert(AK::SoundEngine::UnloadBank(bankName.c_str(),
            nullptr) == AK_Success, "Wwise: Cannot Unload Bank: %s", bankName.c_str());*/
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
            UWarning("Wwise: Cannot Register Game Object");
        }
    }

    void AudioManager::UnRegisterObject(AkGameObjectID obj)
    {
        if (AK::SoundEngine::UnregisterGameObj( obj ) != AK_Success)
        {
            UWarning("Wwise: Cannot Unregister Game Object");
        }
    }

    ListenerIndex AudioManager::GetListener(void)
    {
        auto temp = m_head;
        while (temp)
        {
            if (temp->available)
            {
                temp->available = false;
                return temp->listener;
            }
            temp = temp->next;
        }

        UAssert(false, "Wwise: Requested Too Many Listeners");
        return ListenerIndex::None;
    }

    void AudioManager::FreeListener(ListenerIndex listener)
    {
        auto temp = m_head;
        while (temp)
        {
            if (temp->listener == listener)
            {
                if (temp->available == true)
                {
                    UAssert(false, "Wwise: Trying To Free Unused Listener");
                    return;
                }
                temp->available = true;
                return;
            }

            temp = temp->next;
        }
        UAssert(false, "Wwise: Could Not Free Listeners [Not Valid Index] ");
    }

    void AudioManager::RegisterWwisePlugin(const AkPluginType type, const AkUInt32 company_id, const AkUInt32 plugin_id, AkCreatePluginCallback create_func, AkCreateParamCallback create_param)
    {
        if (AK::SoundEngine::RegisterPlugin( type, company_id, plugin_id, create_func, create_param ))
        {
            UWarning("Wwise: Cannot Register Plugin");
        }
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
