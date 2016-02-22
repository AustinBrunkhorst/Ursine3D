#include "UrsinePrecompiled.h"

#include "JsonImporter.h"
#include "JsonData.h"

namespace ursine
{
    rp::JsonImporter::JsonImporter(void) { }

    resources::ResourceData::Handle rp::JsonImporter::Import(const fs::path &fileName, const ResourceImportContext &context)
    {
        std::string jsonError;
        std::string jsonText;

        URSINE_TODO( "use exceptions instead of assertions" );

        fs::LoadAllText( fileName.string( ), jsonText );

        auto data = std::make_shared<JsonData>( Json::parse( jsonText, jsonError ) );

        UAssert( jsonError.empty( ),
            "Unable to parse Json.\nerror: %s\nfile: %s",
            jsonError.c_str( ),
            fileName.string( ).c_str( )
        );

        return data;
    }
}