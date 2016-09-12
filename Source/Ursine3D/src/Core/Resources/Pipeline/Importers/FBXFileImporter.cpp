/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** FBXFileImporter.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "FBXFileImporter.h"
#include "ModelData.h"
#include "AnimationClipData.h"

#include "CFBXLoader.h"

namespace ursine
{
    rp::FBXFileImporter::FBXFileImporter(void) { }

    rp::FBXFileImporter::~FBXFileImporter(void) { }

    resources::ResourceData::Handle rp::FBXFileImporter::Import(ResourceImportContext &context)
    {
        auto fileName = context.resource->GetSourceFileName( );

        graphics::CFBXLoader importer;

        UAssertCatchable( importer.LoadFBX( fileName.string( ) ),
            "Unable to import FBX file.\nfile: %s",
            fileName.string( ).c_str( )
        );

        auto &animInfo = importer.GetAnimInfo( );
        
        // importing model info check
        // if there is animation, just animation
        if (animInfo.IsValid( ))
            return std::make_shared<AnimationClipData>( animInfo );
        // else export just model
        else
            return std::make_shared<ModelData>( importer.GetModelInfo( ) );
    }
}