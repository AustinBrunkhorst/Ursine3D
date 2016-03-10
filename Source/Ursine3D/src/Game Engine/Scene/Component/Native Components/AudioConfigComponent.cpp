#include "UrsinePrecompiled.h"

#include "AudioConfigComponent.h"

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
        }

        void AudioConfig::invalidateMainBank(void)
        {
            loadResource<resources::AudioData>( m_mainBank );
        }
    }
}