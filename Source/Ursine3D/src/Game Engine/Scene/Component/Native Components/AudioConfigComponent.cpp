#include "UrsinePrecompiled.h"

#include "AudioConfigComponent.h"

#include "AudioSystem.h"
#include "WorldEvent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( AudioConfig );

        AudioConfig::AudioConfig(void)
            : BaseComponent( )
        {
            
        }

        AudioConfig::~AudioConfig(void)
        {
            auto world = GetOwner( )->GetWorld( );

            if (world)
                world->Listener( this )
                    .Off( WORLD_VOLUME_CHANGE, &AudioConfig::onVolumeChange );
        }

        const resources::ResourceReference &AudioConfig::GetMainBank(void)
        {
            return m_mainBank;
        }

        void AudioConfig::SetMainBank(const resources::ResourceReference &mainBank)
        {
            m_mainBank = mainBank;

            if (!resourcesAreAvailable( ))
                return;

            invalidateMainBank( );

            NOTIFY_COMPONENT_CHANGED( "mainBank", m_mainBank );
        }

        void AudioConfig::OnSceneReady(Scene *scene)
        {
            invalidateMainBank( );

            GetOwner( )->GetWorld( )->Listener( this )
                .On( WORLD_VOLUME_CHANGE, &AudioConfig::onVolumeChange );
        }

        void AudioConfig::invalidateMainBank(void)
        {
            loadResource<resources::AudioData>( m_mainBank );
        }

        void AudioConfig::onVolumeChange(EVENT_HANDLER(World))
        {
            EVENT_ATTRS(World, VolumeChangeArgs);

            for (auto &control : volumeControls)
            {
                if (control.outputTypeName == args->outputType)
                {
                    // send an rtpc event to the audio system
                    AudioSystem::SetRealTimeParameter( control.rtpcName, args->volume );
                }
            }
        }
    }
}
