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

#include "Content3DImporter.h"

#include "BuiltInResourceConfig.h"

#include "UModelData.h"
#include "URigData.h"
#include "UAnimationData.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

namespace
{
    struct Content3D
    {
        ursine::resources::UModelData::Handle model;
        ursine::resources::URigData::Handle rig;
        ursine::resources::UAnimationData::Handle animation;
    };
}

namespace ursine
{
    namespace resources
    {
        static void importScene(const aiScene *scene, Content3D &content);
        static void importSceneNode(const aiScene *scene, const aiNode *node, Content3D &content);
        static void importSceneNodeMeshes(const aiScene *scene, const aiNode *node, const UModelData::Handle &output);

        static void importMeshVerts(const aiMesh *mesh, const UMeshData::Handle &output);
        static void importMeshNormals(const aiMesh *mesh, const UMeshData::Handle &output);
        static void importMeshTangents(const aiMesh *mesh, const UMeshData::Handle &output);
        static void importMeshIndices(const aiMesh *mesh, const UMeshData::Handle &output);

        rp::Content3DImporter::Content3DImporter(void) { }

        rp::Content3DImporter::~Content3DImporter(void) { }

        ResourceData::Handle rp::Content3DImporter::Import(ResourceImportContext &context)
        {
            auto fileName = context.resource->GetSourceFileName( ).string( ).c_str( );
            auto flags = aiProcessPreset_TargetRealtime_MaxQuality;

            Assimp::Importer importer;

            UAssertCatchable(
                !importer.ValidateFlags( flags ),
                "Invalid post process flags."
            );

            const aiScene *scene = importer.ReadFile( fileName, flags );

            UAssertCatchable(
                scene == nullptr,
                "Failed to load file: %s",
                importer.GetErrorString( )
            );

            Content3D output;

            importScene( scene, output );

            if (output.model != nullptr)
            {
                // create the model generated resource
                auto rootName = context.resource->GetSourceFileName( );
                auto modelPath = rootName.replace_extension( kResourceTypeModelExtension );

                ResourceWriter writer( modelPath );

                output.model->Write( writer );

                context.AllocateGeneratedResource( modelPath );
            }

            // TODO: [J] Missing importing this
            /*if (scene->HasAnimations())
            {
            // create animation shit
            }*/

            // if (scene->Has[Cameras][Lights][Materials][Textures])

            return std::make_shared<Content3DData>( );
        }

        static void importScene(const aiScene *scene, Content3D &content)
        {
            if (scene->HasMeshes( ))
            {
                content.model = std::make_shared<UModelData>( );
            }

            if (scene->HasAnimations( ))
            {
                content.animation = std::make_shared<UAnimationData>( );
                content.rig = std::make_shared<URigData>( );
            }

            // iterate through all scene nodes and add their objects to the u3dcontent data
            importSceneNode( scene, scene->mRootNode, content );
        }

        static void importSceneNode(const aiScene *scene, const aiNode *node, Content3D &content)
        {
            if (!node)
                return;

            importSceneNodeMeshes( scene, node, content.model );

            for (uint i = 0, n = node->mNumChildren; i < n; ++i)
            {
                importSceneNode( scene, node->mChildren[ i ], content );
            }
        }

        static void importSceneNodeMeshes(const aiScene *scene, const aiNode *node, const UModelData::Handle &output)
        {
            if (node->mNumMeshes == 0)
                return;

            for (uint i = 0, n = node->mNumMeshes; i < n; ++i)
            {
                auto index = node->mMeshes[ i ];
                auto mesh = scene->mMeshes[ index ];
                auto newMesh = std::make_shared<UMeshData>( );

                importMeshVerts( mesh, newMesh );
                importMeshNormals( mesh, newMesh );
                importMeshTangents( mesh, newMesh);
                importMeshIndices( mesh, newMesh );

                output->AddMesh( newMesh );
            }
        }

        static void importMeshVerts(const aiMesh *mesh, const UMeshData::Handle &output)
        {
            if (!mesh->HasPositions( ))
                return;

            output->verts.resize( mesh->mNumVertices );

            for (uint i = 0, n = mesh->mNumVertices; i < n; ++i)
            {
                auto &vert = mesh->mVertices[ i ];

                output->verts[ i ].Set(
                    vert.x, vert.y, vert.z
                );
            }
        }

        static void importMeshNormals(const aiMesh *mesh, const UMeshData::Handle &output)
        {
            if (!mesh->HasNormals( ))
                return;

            output->normals.resize( mesh->mNumVertices );

            for (uint i = 0, n = mesh->mNumVertices; i < n; ++i)
            {
                auto &norm = mesh->mNormals[ i ];

                output->normals[ i ].Set(
                    norm.x, norm.y, norm.z
                );
            }
        }

        static void importMeshTangents(const aiMesh *mesh, const UMeshData::Handle &output)
        {
            if (!mesh->HasTangentsAndBitangents( ))
                return;

            output->tangents.resize( mesh->mNumVertices );
            output->bitangents.resize( mesh->mNumVertices );

            for (uint i = 0, n = mesh->mNumVertices; i < n; ++i)
            {
                auto &tangent = mesh->mTangents[ i ];
                auto &bitangent = mesh->mBitangents[ i ];

                output->tangents[ i ].Set(
                    tangent.x, tangent.y, tangent.z
                );

                output->bitangents[ i ].Set(
                    bitangent.x, bitangent.y, bitangent.z
                );
            }
        }

        static void importMeshIndices(const aiMesh *mesh, const UMeshData::Handle &output)
        {
            if (!mesh->HasFaces( ))
                return;

            output->indices.resize( mesh->mNumFaces * 3 );

            for (uint i = 0, n = mesh->mNumFaces; i < n; ++i)
            {
                auto &face = mesh->mFaces[ i ];
                auto index = i * 3;

                UAssertCatchable(
                    face.mNumIndices == 3, "Face doesn't have 3 indices."
                );

                output->indices[ index ] = face.mIndices[ 0 ];
                output->indices[ index + 1 ] = face.mIndices[ 1 ];
                output->indices[ index + 2 ] = face.mIndices[ 2 ];
            }
        }
    }
}
