/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** JsonImporter.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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

        UAssertCatchable( fs::LoadAllText( fileName.string( ), jsonText ),
            "Unable to open file.\nfile: %s",
            fileName.string( ).c_str( )
        );

        auto data = std::make_shared<JsonData>( Json::parse( jsonText, jsonError ) );

        UAssertCatchable( jsonError.empty( ),
            "Unable to parse Json.\nerror: %s\nfile: %s",
            jsonError.c_str( ),
            fileName.string( ).c_str( )
        );

        return data;
    }
}