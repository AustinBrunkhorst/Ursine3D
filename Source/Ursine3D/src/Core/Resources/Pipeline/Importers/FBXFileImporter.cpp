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
        
        auto sourceFileDirectory = fileName.parent_path( );

        //// what if i want to import just model or just animation - to reduce memory for resources
        //// like we separate jdl and jani. we don't need to import same model/rig several times.
        //if( std::string::npos == sourceFileDirectory.string().find("Models") )

        auto animInfo = importer.GetAnimInfo( );
        
        // importing model info check
        // if there is animation, just animation
        if ( animInfo.IsThereAAnimation( ) )
            return std::make_shared<AnimationClipData>( animInfo );
        // else export just model
        else
            return std::make_shared<ModelData>( importer.GetModelInfo( ) );
    }
}