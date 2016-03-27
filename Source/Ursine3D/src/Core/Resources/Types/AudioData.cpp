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
            BinaryData initData,
            BinaryData bankData
        )
            : m_events( events )
            , m_initData( std::move( initData ) )
            , m_bankData( std::move( bankData ) )
        {
            auto *audioManager = GetCoreSystem( AudioManager );

            if (audioManager)
                audioManager->LoadBank( *this, m_initID, m_bankID );
        }

        AudioData::AudioData(const AudioData &rhs)
            : m_events( rhs.m_events )
            , m_initData( BinaryData::Copy( rhs.m_initData ) )
            , m_bankData( BinaryData::Copy( rhs.m_bankData ) )
            , m_initID( rhs.m_initID )
            , m_bankID( rhs.m_bankID )
        {

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

        const BinaryData &AudioData::GetInitData(void) const
        {
            return m_initData;
        }

        const BinaryData &AudioData::GetBankData(void) const
        {
            return m_bankData;
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
                output.Write( e );

            output.Write( m_initData );
            output.Write( m_bankData );
        }

        meta::Type AudioData::GetReaderType(void)
        {
            return typeof( AudioReader );
        }
    }
}