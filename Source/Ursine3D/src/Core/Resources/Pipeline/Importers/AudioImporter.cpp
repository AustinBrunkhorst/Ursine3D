#include "UrsinePrecompiled.h"

#include "AudioImporter.h"
#include "AudioData.h"

#include "BuiltInResourceConfig.h"

#include <boost/regex.hpp>

namespace
{
    // name of the init bank required in all wwise generated stuffs
    const auto kWWiseInitBankName = "Init.bnk";

    const auto kWWiseBankExtension = ".bnk";
    const auto kWWiseBankEventsExtension = ".txt";

    const auto kWWiseEventsHeader = std::string( "Event" );

    void extractBankEvents(const ursine::fs::path &bankName, ursine::resources::AudioData::EventList &outEvents);
    void loadBinary(const ursine::fs::path &fileName, ursine::BinaryData &outData);
}

namespace ursine
{
    rp::AudioImporter::AudioImporter(void) { }

    rp::AudioImporter::~AudioImporter(void) { }

    resources::ResourceData::Handle rp::AudioImporter::Import(ResourceImportContext &context)
    {
        auto &fileName = context.resource->GetSourceFileName( );
        auto fileDirectory = fileName.parent_path( );

        // init bank file
        auto initBankFile = fileName / kWWiseInitBankName;

        UAssertCatchable( exists( initBankFile ),
            "Missing '%s'.",
            kWWiseInitBankName
        );

        // bank file with the same name as the directory resource
        auto bankFile = (fileName / fileName.stem( )).replace_extension( kWWiseBankExtension );

        UAssertCatchable( exists( bankFile ),
            "Missing '%s'.",
            bankFile.filename( ).string( ).c_str( )
        );

        AudioData::EventList eventNames;

        extractBankEvents( bankFile, eventNames );

        for (auto &event : eventNames)
        {
            Json data = Json::object {
                { "project", to_string( context.resource->GetGUID( ) ) },
                { "event", event }
            };

            auto eventFileName = (fileDirectory / event)
                .replace_extension( kResourceTypeAudioEventExtension );

            fs::WriteAllText( eventFileName.string( ), data.dump( true ) );

            context.AllocateGeneratedResource( eventFileName );
        }

        BinaryData initData;

        loadBinary( initBankFile, initData );

        BinaryData bankData;

        loadBinary( bankFile, bankData );

        auto resource = std::make_shared<AudioData>( 
            eventNames, 
            std::move( initData ),
            std::move( bankData )
        );

        return resource;
    }
}

namespace
{
    void extractBankEvents(const ursine::fs::path &bankName, ursine::resources::AudioData::EventList &outEvents)
    {
        auto eventsFile = change_extension( bankName, kWWiseBankEventsExtension );

        if (!exists( eventsFile ))
        {
            // this isn't fatal, but should create a warning
            UWarning( "Bank missing events file '%s'.",
                eventsFile.string( ).c_str( )
            );

            return;
        }

        std::ifstream eventStream( eventsFile.string( ), std::ios::in );

        UAssertCatchable( eventStream.is_open( ),
            "Unable to load bank events file.\nfile: %s",
            eventsFile.string( ).c_str( )
        );

        auto inEvents = false;

        std::string line;

        static const boost::regex eventLineRegex( "^\t+([^\t]+)\t([^\t]+)\t{3}([^\t]+)\t" );

        while (getline( eventStream, line ))
        {
            if (!inEvents && line.substr( 0, kWWiseEventsHeader.size( ) ) == kWWiseEventsHeader)
            {
                inEvents = true;
            }
            else if (inEvents)
            {
                auto flags = boost::match_default | boost::format_all;

                boost::match_results<std::string::const_iterator> match;

                if (boost::regex_search( 
                        line.cbegin( ), 
                        line.cend( ), 
                        match, 
                        eventLineRegex,
                        flags 
                    )
                )
                {
                    outEvents.emplace_back( match[ 2 ].str( ) );
                }
                else
                {
                    // finished finding all events
                    return;
                }
            }
        }
    }

    void loadBinary(const ursine::fs::path &fileName, ursine::BinaryData &outData)
    {
        UAssertCatchable( ursine::fs::LoadAllBinary( fileName.string( ), outData ),
            "Unable to load audio file.\nfile: %s",
            fileName.string( ).c_str( )
        );
    }
}