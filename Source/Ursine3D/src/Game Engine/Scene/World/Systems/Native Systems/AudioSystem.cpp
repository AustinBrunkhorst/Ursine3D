/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AudioSystem.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "AudioSystem.h"

#include "AudioConfigComponent.h"
#include "AudioEmitterComponent.h"
#include "ListenerComponent.h"

#include "AudioEventInfo.h"

namespace ursine
{
    namespace ecs
    {
        ENTITY_SYSTEM_DEFINITION( AudioSystem );

        AudioSystem::AudioSystem(World *world)
            : EntitySystem( world )
            , m_nextEmptyID( AUDIO_UNALLOCATED_ID + 1 )
        {
            m_audioMan = GetCoreSystem( AudioManager );
        }

        void AudioSystem::CreateAudioObject(AkGameObjectID &id)
        {
            id = m_nextEmptyID++;
            m_audioMan->RegisterObject( id, 0 );
        }

        void AudioSystem::DeleteAudioObject(AkGameObjectID &id)
        {
            m_audioMan->UnRegisterObject( id );
        }

        void AudioSystem::DeleteAllAudioObjects(void)
        {
            if (AK::SoundEngine::UnregisterAllGameObj(  ) != AK_Success)
            {
                UWarning("Wwise: Cannot Unregister ALL game objects");
            }
        }

        void AudioSystem::OnInitialize(void)
        {
            m_world->Listener( this )
                .On( WORLD_UPDATE, &AudioSystem::onUpdate )
                .On( WORLD_ENTITY_COMPONENT_ADDED, &AudioSystem::onComponentAdded )
                .On( WORLD_ENTITY_COMPONENT_REMOVED, &AudioSystem::onComponentRemoved );
        }

        void AudioSystem::OnSceneReady(Scene *scene)
        {
            auto settings = m_world->GetSettings( );

            if (!settings->HasComponent<AudioConfig>( ))
                settings->AddComponent<AudioConfig>( );
        }

        void AudioSystem::OnRemove(void)
        {
            StopAllSounds();
            DeleteAllAudioObjects( );

            m_world->Listener( this )
                .Off( WORLD_UPDATE, &AudioSystem::onUpdate )
                .Off( WORLD_ENTITY_COMPONENT_ADDED, &AudioSystem::onComponentAdded )
                .Off( WORLD_ENTITY_COMPONENT_REMOVED, &AudioSystem::onComponentRemoved );
        }

        void AudioSystem::onComponentAdded(EVENT_HANDLER(World))
        {
            EVENT_ATTRS(World, ComponentEventArgs);

            if (args->component->Is<AudioEmitter>( ))
            {
                m_emitters.emplace(
                    args->entity,
                    static_cast<AudioEmitter*>( const_cast<Component*>( args->component ) )
                );

                auto emitter = m_emitters[ args->entity ];
                auto &handle = emitter->m_handle;

                CreateAudioObject( handle );
                AssignListener( handle, m_emitters[ args->entity ]->GetListenerMask( ) );

                emitter->SetAttenuationScalingFactor( emitter->GetAttenuationScalingFactor( ) );
            }

            else if (args->component->Is<AudioListener>( ))
            {
                m_listeners.emplace(
                    args->entity,
                    static_cast<AudioListener*>( const_cast<Component*>( args->component ) )
                );
            }
        }

        void AudioSystem::onComponentRemoved(EVENT_HANDLER(World))
        {
            EVENT_ATTRS(World, ComponentEventArgs);

            if (args->component->Is<AudioEmitter>( ))
            {
                auto search = m_emitters.find( args->entity );

                if (search != m_emitters.end( ))
                {
                    auto &emitter = m_emitters[ args->entity ];
                    auto handle = emitter->m_handle;

                    // post all remaining events
                    while (!emitter->EmptyEvent( ))
                    {
                        auto postedEvent = emitter->GetEvent( );
                        PostAudioEventInfo( postedEvent, handle );
                        emitter->PopEvent( );
                    }

                    DeleteAudioObject( handle );
                    m_emitters.erase( search );
                }
            }

            else if (args->component->Is<AudioListener>( ))
            {
                auto search = m_listeners.find( args->entity );

                if (search != m_listeners.end( ))
                {
                    m_listeners.erase( search );
                }
            }
        }

        void AudioSystem::onUpdate(EVENT_HANDLER(World))
        {
            for (auto &listener : m_listeners)
            {
                auto index = listener.second->GetListenerIndex( );

                auto entity = listener.first;

                if (!entity || index == ListenerIndex::NONE)
                    continue;

                auto trans = entity->GetTransform();

                    SetListener3DPosition( trans->GetForward( ), trans->GetUp( ),
                    trans->GetWorldPosition( ), index );
            }

            // walk through all emitters
            // update positions
            // Post Events
            for (auto &emitter : m_emitters)
            {
                auto &handle = emitter.second->m_handle;
                auto entity = emitter.first;
                auto eventQ = emitter.second;
                auto &stopSoundFlag = emitter.second->checkStopFlag( );
                auto mask = emitter.second->GetListenerMask( );

                // I need to find out a way to access all of the events from the emitter's queues

                if (!entity)
                    continue;

                auto trans = entity->GetTransform( );

                    SetObject3DPosition( handle, trans->GetWorldPosition( ),
                        trans->GetForward( ) );

                    AssignListener( handle, mask );

                while (!eventQ->EmptyEvent( ))
                {
                    auto postedEvent = eventQ->GetEvent( );
                    PostAudioEventInfo( postedEvent, handle );
                    eventQ->PopEvent( );
                }

                if (stopSoundFlag)
                {
                    StopAllSounds( handle );
                    stopSoundFlag = false;
                }
            }
        }

        void AudioSystem::SetRealTimeParameter(const std::string param, const float value, AkGameObjectID id)
        {
            if (AK::SoundEngine::SetRTPCValue( param.c_str( ), ( AkRtpcValue )value, id ) != AK_Success)
            {
                UWarning( "Wwise: Cannot Set RTPC value: %s", param.c_str( ) );
            }
        }

        void AudioSystem::StopAllSounds(void)
        {
            AK::SoundEngine::StopAll( );
        }

        void AudioSystem::StopAllSounds(AkGameObjectID id)
        {
            AK::SoundEngine::StopAll( id );
        }

        void AudioSystem::PostAudioEvent(const std::string param, AkGameObjectID id)
        {
            auto manager = GetCoreSystem( AudioManager );
            manager->PlayEvent( param, id );
        }

        void AudioSystem::PostAudioEventInfo(AudioEvent::Handle event, AkGameObjectID id)
        {
            event->ApplyParams( id );
        }

        void AudioSystem::AssignListener(AkGameObjectID obj, ListenerMask listeners)
        {
            if (AK::SoundEngine::SetActiveListeners( obj, static_cast<AkUInt32>( listeners ) ) != AK_Success)
            {
                UWarning( "Wwise: Cannot Set Active Listeners" );
            }
        }

        void AudioSystem::SetListener3DPosition(const AkVector orientation_forward,
            const AkVector orientation_up, const AkVector position, const ListenerMask listeners)
        {
            AkListenerPosition listenerPosition;
            // up and forward have to be orthogonal and normalized
            listenerPosition.OrientationTop = orientation_up;
            listenerPosition.OrientationFront = orientation_forward;
            listenerPosition.Position = position;

            if (AK::SoundEngine::SetListenerPosition( listenerPosition, static_cast<AkUInt32>( listeners ) ) != AK_Success)
            {
                UWarning( "Wwise: Cannot Set Listener Postion" );
            }
        }

        void AudioSystem::SetListener3DPosition(
            const SVec3 orientationForward,
            const SVec3 orientationUp,
            const SVec3 position,
            const ListenerIndex index
        )
        {
            AkListenerPosition listenerPosition;
            // up and forward have to be orthogonal and normalized
            listenerPosition.OrientationTop = orientationUp.ToWwise( );
            listenerPosition.OrientationFront = orientationForward.ToWwise( );
            listenerPosition.Position = position.ToWwise( );

            AkUInt32 listener = static_cast<AkUInt32>( index );

            if (AK::SoundEngine::SetListenerPosition( listenerPosition, listener ) != AK_Success)
            {
                UWarning( "Wwise: Cannot Set Listener Postion" );
            }
        }

        void AudioSystem::SetObject3DPosition(AkGameObjectID obj, const AkSoundPosition position)
        {
            if (AK::SoundEngine::SetPosition( obj, position ) != AK_Success)
            {
                UWarning( "Wwise: Cannot Set Object 3D Position" );
            }
        }

        void AudioSystem::SetMultipleObject3DPosition(
            AkGameObjectID obj,
            const AkSoundPosition *positions,
            AkUInt16 numPositions,
            AK::SoundEngine::MultiPositionType type
        )
        {
            // MultiPositionType_MultiSources     
            // Simulate multiple sources in one sound playing, adding volumes. 
            // For instance, all the torches on your level emitting using only one sound.

            // MultiPositionType_MultiDirections
            // Simulate one sound coming from multiple directions. Useful for repositioning 
            // sounds based on wall openings or to simulate areas like forest or rivers

            if (SetMultiplePositions( obj, positions, numPositions, type ) != AK_Success)
            {
                UWarning( "Wwise: Cannot Set Mutiple Object 3D Position" );
            }
        }

        void AudioSystem::SetSoundObstructionAndOcclusion(
            AkGameObjectID obstruction,
            const AkUInt32 listeners,
            const AkReal32 obstructionLevel,
            const AkReal32 occlusion_level
        )
        {
            if (AK::SoundEngine::SetObjectObstructionAndOcclusion( obstruction, listeners,
                obstructionLevel, occlusion_level ) != AK_Success)
            {
                UWarning( "Wwise: Cannot Set Obstruction and Occlusion" );
            }
        }

        void AudioSystem::SetGameState(const std::string &group, const std::string &state)
        {
            // global state change
            if (AK::SoundEngine::SetState( group.c_str( ), state.c_str( ) ))
            {
                // Commenting this out so that we don't get frame hitches
                // - Jordan
                // UWarning( "Wwise: Cannot Set State" );
            }
        }

        void AudioSystem::SetObjectSwitch(const std::string &group, const std::string &state, AkGameObjectID obj)
        {
            // specific states for each object
            if (AK::SoundEngine::SetSwitch( group.c_str( ), state.c_str( ), obj ))
            {
                UWarning( "Wwise: Cannot Set Switch" );
            }
        }

        void AudioSystem::SetTrigger(const std::string &name, AkGameObjectID obj)
        {
            if (AK::SoundEngine::PostTrigger( name.c_str( ), obj ))
            {
                UWarning( "Wwise: Cannot Post Trigger" );
            }
        }

        void AudioSystem::SetObject3DPosition(AkGameObjectID obj, const SVec3 position, const SVec3 orientation)
        {
            AkSoundPosition pos;
            pos.Position = position.ToWwise( );
            pos.Orientation = orientation.ToWwise( );
            if (AK::SoundEngine::SetPosition( obj, pos ) != AK_Success)
            {
                UWarning( "Wwise: Cannot Set Object 3D Position" );
            }
        }
    }
}
