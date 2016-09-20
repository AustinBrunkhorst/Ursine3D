/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** FBXImporter.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "FBXImporter.h"

#include "U3DContentData.h"

namespace ursine
{
    rp::FBXImporter::FBXImporter(void) { }

    rp::FBXImporter::~FBXImporter(void) { }

    resources::ResourceData::Handle rp::FBXImporter::Import(ResourceImportContext &context)
    {
        auto output = std::make_shared<U3DContentData>( );
        auto fileName = context.resource->GetSourceFileName( );

        UAssertCatchable( importFBX( fileName.string( ), output.get( ) ),
            "Unable to import FBX file.\nfile: %s",
             fileName.string( ).c_str( )
        );

        // TODO: Return the intermediary data to the processor
        return output;
    }

    bool rp::FBXImporter::importFBX(const std::string &fileName, U3DContentData *content)
    {
        // TODO: Import FBX to intermediary format (mesh, rig, anims, ect)
    }
}