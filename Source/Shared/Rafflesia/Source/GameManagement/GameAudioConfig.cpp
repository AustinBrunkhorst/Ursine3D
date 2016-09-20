#include "Precompiled.h"

#include "GameAudioConfig.h"

#include <AudioSystem.h>
#include <WorldEvent.h>
#include <Scene.h>

using namespace ursine;

NATIVE_COMPONENT_DEFINITION( GameAudioConfig );

GameAudioConfig::GameAudioConfig(GameManager *manager)
    : BaseComponent( )
    , m_manager( manager ) { }

GameAudioConfig::GameAudioConfig(void)
    : BaseComponent( )
    , m_manager( nullptr ) { }

GameAudioConfig::~GameAudioConfig(void)
{

}

const resources::ResourceReference &GameAudioConfig::GetMainBank(void)
{
    return m_mainBank;
}

void GameAudioConfig::SetMainBank(const resources::ResourceReference &mainBank)
{
    m_mainBank = mainBank;

    if (m_manager)
        invalidateMainBank( );

    NOTIFY_COMPONENT_CHANGED( "mainBank", m_mainBank );
}

void GameAudioConfig::invalidateMainBank(void)
{
    m_mainBank.Load<resources::AudioData>( m_manager->GetContext( )->GetScene( )->GetResourceManager( ) );
}

void GameAudioConfig::onVolumeChange(EVENT_HANDLER(World))
{
    EVENT_ATTRS(ecs::World, ecs::VolumeChangeArgs);

    for (auto &control : volumeControls)
    {
        if (control.outputTypeName == args->outputType)
        {
            // send an rtpc event to the audio system
            ecs::AudioSystem::SetRealTimeParameter( control.rtpcName, args->volume );
        }
    }
}