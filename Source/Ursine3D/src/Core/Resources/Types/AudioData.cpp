#include "UrsinePrecompiled.h"

#include "AudioData.h"
#include "AudioReader.h"

#include "AudioManager.h"

namespace ursine
{
    namespace resources
    {
        AudioData::AudioData(
            const EventList &events, 
            void *initBytes,
            size_t initByteSize, 
            void *bankBytes,
            size_t bankSize
        )
            : m_events( events )
            , m_initBytes( new uint8[ initByteSize ] )
            , m_initByteSize( initByteSize )
            , m_bankBytes( new uint8[ bankSize ] )
            , m_bankSize( bankSize )
        {
            std::memcpy( m_initBytes, initBytes, m_initByteSize );
            std::memcpy( m_bankBytes, bankBytes, m_bankSize );

            auto *audioManager = GetCoreSystem( AudioManager );

            if (audioManager)
                audioManager->LoadBank( *this, m_initID, m_bankID );
        }

        AudioData::~AudioData(void)
        {
            auto *audioManager = GetCoreSystem( AudioManager );

            if (audioManager)
                audioManager->UnloadBank( *this );
        }

        const AudioData::EventList &AudioData::GetEvents(void) const
        {
            return m_events;
        }

        void *AudioData::GetInitBytes(void) const
        {
            return m_initBytes;
        }

        size_t AudioData::GetInitByteSize(void) const
        {
            return m_initByteSize;
        }

        void *AudioData::GetBankBytes(void) const
        {
            return m_bankBytes;
        }

        size_t AudioData::GetBankSize(void) const
        {
            return m_bankSize;
        }

        AkBankID AudioData::GetInitID(void) const
        {
            return m_initID;
        }

        AkBankID AudioData::GetBankID(void) const
        {
            return m_bankID;
        }

        void AudioData::Write(pipeline::ResourceWriter &output)
        {
            output.Write( static_cast<unsigned>( m_events.size( ) ) );

            for (auto &e : m_events)
                output.WriteString( e );

            output.Write( m_initByteSize );
            output.WriteBytes( m_initBytes, m_initByteSize );

            output.Write( m_bankSize );
            output.WriteBytes( m_bankBytes, m_bankSize );
        }

        meta::Type AudioData::GetReaderType(void)
        {
            return typeof( AudioReader );
        }
    }
}