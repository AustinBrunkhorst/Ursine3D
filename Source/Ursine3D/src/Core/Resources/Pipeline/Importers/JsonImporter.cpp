#include "UrsinePrecompiled.h"

#include "JsonImporter.h"
#include "JsonData.h"

namespace ursine
{
    rp::JsonImporter::JsonImporter(void) { }

    resources::ResourceData::Handle rp::JsonImporter::Import(ResourceImportContext &context)
    {
        std::string jsonError;
        std::string jsonText;

        auto fileName = context.resource->GetSourceFileName( );

        fs::LoadAllText( fileName.string( ), jsonText );

        auto data = std::make_shared<JsonData>( Json::parse( jsonText, jsonError ) );

        UAssertCatchable( jsonError.empty( ),
            "Unable to parse Json.\nerror: %s\nfile: %s",
            jsonError.c_str( ),
            fileName.string( ).c_str( )
        );

        return data;
    }
}