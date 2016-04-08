/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AudioEmitterComponent.cpp
**
** Author:
** - Jason Burch - j.orion@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "AudioEmitterComponent.h"

#include "AudioItemEventData.h"

namespace ursine
{
    using namespace resources;

    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION(AudioEmitter);

        AudioEmitter::AudioEmitter(void)
            : BaseComponent( )
            , m_maskChanged( false )
            , m_listenerMask( ListenerMask::NONE )  { }

        AudioEmitter::~AudioEmitter(void)
        {
            AudioComponentBase::OnRemove( GetOwner( ) );
        }

        void AudioEmitter::OnInitialize(void)
        {
            AudioComponentBase::OnInitialize( GetOwner( ) );
        }

        ListenerMask AudioEmitter::GetListenerMask(void) const
        {
            return m_listenerMask;
        }

    #if defined(URSINE_WITH_EDITOR)

        void AudioEmitter::PushTestSound(void)
        {
            PushEvent( m_testEvent );
        }

    #endif

        void AudioEmitter::PushEvent(const AudioEvent::Handle event)
        {
            m_events.push( event );
        }

        void AudioEmitter::PushEvent(const resources::ResourceReference &eventResource)
        {
            auto world = GetOwner( )->GetWorld( );

            if (!world)
                return;

            auto scene = world->GetOwner( );

            if (!scene)
                return;

            auto event = eventResource.Load<AudioItemEventData>( scene->GetResourceManager( ) );

            if (!event)
                return;

            auto newEvent = std::make_shared<AudioGeneric>( );

            newEvent->name = event->GetEvent( );

            PushEvent( newEvent );
        }

        AudioEvent::Handle AudioEmitter::GetEvent(void)
        {
            return m_events.front( );
        }

        void AudioEmitter::PopEvent(void)
        {
            m_events.pop( );
        }

        bool AudioEmitter::EmptyEvent()
        {
            return m_events.empty( );
        }

        bool AudioEmitter::checkMask()
        {
            return m_maskChanged;
        }

        void AudioEmitter::ResetMaskFlag()
        {
            m_maskChanged = false;
        }

        void AudioEmitter::SetListenerMask(ListenerMask mask)
        {
            m_listenerMask = mask;

            m_maskChanged = true;

            NOTIFY_COMPONENT_CHANGED( "listenerMask", m_listenerMask );
        }

        const ResourceReference &AudioEmitter::GetTestEvent(void) const
        {
            return m_testEvent;
        }

        void AudioEmitter::SetTestEvent(const ResourceReference &audioEvent)
        {
            m_testEvent = audioEvent;

            NOTIFY_COMPONENT_CHANGED( "TestEvent", m_testEvent );
        }
    }
}