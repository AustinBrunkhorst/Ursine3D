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
    static void InitializeSdkObjects(FbxManager *&manager, FbxScene *&scene)
    {
        // The first thing to do is to create the FBX Manager
        // which is the object allocator for almost all the classes in the SDK
        manager = FbxManager::Create( );

        UAssertCatchable( manager,
            "Error: Unable to create FbxManager."
        );

        //Create an IOSettings object. This object holds all import/export settings.
        FbxIOSettings *ios = FbxIOSettings::Create( manager, IOSROOT );
        manager->SetIOSettings( ios );

        //Create an FBX scene. This object holds most objects imported/exported from/to files.
        scene = FbxScene::Create( manager, "FbxScene");

        UAssertCatchable( scene,
            "Error: Unable to create scene."
        );
    }

    static void DestroySdkObjects(FbxManager *&manager, FbxScene *&scene)
    {
        manager->Destroy( );
        manager = nullptr;
        scene = nullptr;
    }

    static void LoadFbx(const char *fileName, FbxManager *manager, FbxScene *scene)
    {
        // Setup importer
        FbxImporter *importer = FbxImporter::Create( manager, "FbxImporter" );

        UAssertCatchable( importer->Initialize( fileName, -1, manager->GetIOSettings( ) ),
            "Error: FbxImporter setup failed with %s file (message: %s).",
            fileName, importer->GetStatus( ).GetErrorString( )
        );

        // Import the file
        // TODO: [J] Look into optimizing this with fbxIOSettings?
        UAssertCatchable( importer->Import( scene ),
            "Error: FbxImporter failed to import the file %s (message: %s).",
            fileName, importer->GetStatus( ).GetErrorString( )
        );

        importer->Destroy( );
    }

    rp::FBXImporter::FBXImporter(void) { }

    rp::FBXImporter::~FBXImporter(void) { }

    resources::ResourceData::Handle rp::FBXImporter::Import(ResourceImportContext &context)
    {
        // TODO: [J] Have this happen in a seperate process (not thread).
        // Have it be part of the "ResourceImporterConfig", a toggle for threaded/child-processes

        auto output = std::make_shared<U3DContentData>( );
        auto fileName = context.resource->GetSourceFileName( ).string( ).c_str( );
        
        UAssertCatchable( importFBX( fileName, output.get( ) ),
            "Unable to import FBX file.\nfile: %s",
             fileName
        );

        return output;
    }

    bool rp::FBXImporter::importFBX(const char *fileName, U3DContentData *content)
    {
        FbxManager *manager = nullptr;
        FbxScene *scene = nullptr;

        InitializeSdkObjects( manager, scene );

        LoadFbx( fileName, manager, scene );

        // TODO: [J] This is shit, use Assimp instead.
        buildModel( scene, content->model );

        DestroySdkObjects( manager, scene );
    }

    void rp::FBXImporter::buildModel(FbxScene *scene, UModelData &modelData)
    {
        for (int i = 0, n = scene->GetGeometryCount( ); i < n; ++i)
        {
            FbxGeometry *geom = scene->GetGeometry( i );

            // We only care about meshes
            if (!geom->GetClassId( ).Is( FbxMesh::ClassId ))
            {
                continue;
            }

            FbxMesh *mesh = static_cast<FbxMesh*>( geom );
            FbxLayer *layer = mesh->GetLayer( 0 );
            UMeshData *meshData = new UMeshData( );

            getVertsAndIndices( mesh, meshData );
            getNormalBinormalAndTangents( layer, meshData );

            modelData.AddMesh( meshData );
        }
    }

    void rp::FBXImporter::getVertsAndIndices(FbxMesh* mesh, UMeshData* meshData)
    {
        // verts
        {
            auto verts = mesh->GetControlPoints( );
            auto numVerts = mesh->GetControlPointsCount();
            auto &dest = meshData->verts;

            dest.resize( numVerts );

            for (int i = 0; i < numVerts; ++i)
            {
                auto &vert = verts[ i ];

                dest[ i ].Set( vert[ 0 ], vert[ 1 ], vert[ 2 ] );
            }
        }

        // indices
        {
            auto indices = mesh->GetPolygonVertices( );
            auto numIndices = mesh->GetPolygonVertexCount( );
            auto &dest = meshData->indices;

            dest.resize( numIndices );

            for (int i = 0; i < numIndices; ++i)
            {
                dest[ i ] = indices[ i ];

                // TODO: [J] Might have to swap dat shit
            }
        }
    }

    void rp::FBXImporter::getNormalBinormalAndTangents(FbxMesh *mesh, UMeshData* meshData)
    {
        // normals
        FbxGeometryElementNormal *normalElement = mesh->GetElementNormal( );

        if (!normalElement)
        {
            // TODO sdsdfsdf
            return;
        }

        switch (normalElement->GetMappingMode( ))
        {
            case FbxGeometryElement::eByControlPoint:
            {
                
            }
        }
    }

}
