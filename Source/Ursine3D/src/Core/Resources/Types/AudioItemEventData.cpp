#include "UrsinePrecompiled.h"

#include "AudioItemEventData.h"
#include "AudioItemEventReader.h"

namespace ursine
{
    namespace resources
    {
        AudioItemEventData::AudioItemEventData(const Json &data)
        {
            auto &projectData = data[ "project" ];
            auto &eventData = data[ "event" ];

            UAssertCatchable( projectData.is_string( ) && eventData.is_string( ),
                "Invalid JSON screen structure."    
            );

            m_project = GUIDStringGenerator( )( projectData.string_value( ) );
            m_event = eventData.string_value( );
        }

        AudioItemEventData::AudioItemEventData(const GUID &project, const std::string &event)
            : m_project( project )
            , m_event( event ) { }

        const GUID &AudioItemEventData::GetProject(void) const
        {
            return m_project;
        }

        const std::string &AudioItemEventData::GetEvent(void) const
        {
            return m_event;
        }

        void AudioItemEventData::Write(pipeline::ResourceWriter &output)
        {
            Json screenData = Json::object {
                { "project", to_string( m_project ) },
                { "event", m_event }
            };

            output.Write( screenData.dump( false ) );
        }

        meta::Type AudioItemEventData::GetReaderType(void)
        {
            return typeof( AudioItemEventReader );
        }
    }
}