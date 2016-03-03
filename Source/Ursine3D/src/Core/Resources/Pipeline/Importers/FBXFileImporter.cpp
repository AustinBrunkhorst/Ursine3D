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
        
        auto animInfo = importer.GetAnimInfo( );
        
        // importing model info check
        URSINE_TODO("NEED TO CHECK IF THERE IS ANIMATION INFO IN SOME OTHER WAY")
        //if (animInfo)
        {
            fs::path clipFileName = animInfo.name + ".uanim";
        
            auto clipPath = sourceFileDirectory / clipFileName;
        
            ResourceWriter clipWriter( clipPath );
            
            UAssertCatchable( clipWriter.IsOpen( ),
                "Unable to write clip.\nfile: %s",
                clipPath.string( ).c_str( )
            );
            
            auto animclipdata = std::make_shared<AnimationClipData>( animInfo );

            animclipdata->Writing( clipWriter );
            
            context.AddGeneratedResource( clipPath );
        }

        return std::make_shared<ModelData>( importer.GetModelInfo( ) );
    }
}