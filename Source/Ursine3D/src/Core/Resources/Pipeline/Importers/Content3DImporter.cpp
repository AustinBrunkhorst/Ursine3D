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
        std::vector<ursine::resources::UAnimationData::Handle> animations;
    };
}

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            ResourceData::Handle Content3DReader::Read(ResourceReader &intput)
            {
                return std::make_shared<Content3DData>();
            }

            meta::Type Content3DData::GetReaderType(void)
            {
                return typeof(Content3DReader);
            }

            void Content3DData::Write(pipeline::ResourceWriter &output)
            {
                
            }
        }

        static void importScene(const aiScene *scene, Content3D &content);
        static void importSceneNode(const aiScene *scene, const aiNode *node, Content3D &content);
        static void importSceneNodeMeshes(const aiScene *scene, const aiNode *node, Content3D &content);

        static void importMeshVerts(const aiMesh *mesh, const UMeshData::Handle &output);
        static void importMeshNormals(const aiMesh *mesh, const UMeshData::Handle &output);
        static void importMeshTangents(const aiMesh *mesh, const UMeshData::Handle &output);
        static void importMeshIndices(const aiMesh *mesh, const UMeshData::Handle &output);
        static void importMeshBonesAndWeights(const aiNode *node, const aiMesh *mesh, 
                                              const UMeshData::Handle &meshOutput, const URigData::Handle &rigOutput);

        static void importSceneAnimations(const aiScene *scene, Content3D &content);

        rp::Content3DImporter::Content3DImporter(void) { }

        rp::Content3DImporter::~Content3DImporter(void) { } 

        ResourceData::Handle rp::Content3DImporter::Import(ResourceImportContext &context)
        {
            auto fileName = context.resource->GetSourceFileName( ).string( );
            auto flags =
                aiProcess_CalcTangentSpace |
                aiProcess_Triangulate |
                aiProcess_JoinIdenticalVertices |
                aiProcess_OptimizeMeshes |
                aiProcess_FindInstances |
                aiProcess_PreTransformVertices |
                aiProcess_FindInvalidData |
                aiProcess_SortByPType |
                aiProcess_ValidateDataStructure |
                aiProcess_LimitBoneWeights |
                aiProcess_GenSmoothNormals;

            Assimp::Importer importer;

            importer.SetPropertyInteger(AI_CONFIG_PP_LBW_MAX_WEIGHTS, 4);

            UAssertCatchable(
                importer.ValidateFlags( flags ),
                "Invalid post process flags."
            );

            const aiScene *scene = importer.ReadFile( fileName.c_str( ), flags );

            UAssertCatchable(
                scene != nullptr,
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

            if (output.rig != nullptr)
            {
                // create the rig generated resource
                auto rootName = context.resource->GetSourceFileName( );
                auto rigPath = rootName.replace_extension( kResourceTypeRigExtension );

                ResourceWriter writer( rigPath );

                output.rig->Write( writer );

                context.AllocateGeneratedResource( rigPath );
            }

            for (auto &animHandle : output.animations)
            {
                auto *anim = static_cast<UAnimationData*>( animHandle.get( ) );

                auto rootName = context.resource->GetSourceFileName( );
                auto animPath = rootName.append( "@" + anim->name );
                animPath.replace_extension( kResourceTypeAnimationExtension );

                ResourceWriter writer( animPath );

                animHandle->Write( writer );

                context.AllocateGeneratedResource( animPath );
            }

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
                for (uint i = 0, n = scene->mNumAnimations; i < n; ++i)
                    content.animations.emplace_back( std::make_shared<UAnimationData>( ) );

                content.rig = std::make_shared<URigData>( );
            }

            // iterate through all scene nodes and add their objects to the u3dcontent data
            importSceneNode( scene, scene->mRootNode, content );
            importSceneAnimations( scene, content );
        }

        static void importSceneNode(const aiScene *scene, const aiNode *node, Content3D &content)
        {
            if (!node)
                return;

            importSceneNodeMeshes( scene, node, content );

            for (uint i = 0, n = node->mNumChildren; i < n; ++i)
            {
                importSceneNode( scene, node->mChildren[ i ], content );
            }
        }

        static void importSceneNodeMeshes(const aiScene *scene, const aiNode *node, Content3D &content)
        {
            if (node->mNumMeshes == 0)
                return;

            for (uint i = 0, n = node->mNumMeshes; i < n; ++i)
            {
                auto index = node->mMeshes[ i ];
                auto mesh = scene->mMeshes[ index ];
                auto newMesh = std::make_shared<UMeshData>( );

                newMesh->SetName( node->mName.C_Str( ) );

                importMeshVerts( mesh, newMesh );
                importMeshNormals( mesh, newMesh );
                importMeshTangents( mesh, newMesh);
                importMeshIndices( mesh, newMesh );
                importMeshBonesAndWeights( node, mesh, newMesh, content.rig );

                content.model->AddMesh( newMesh );
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

        static void importMeshBonesAndWeights(const aiNode *node, const aiMesh *mesh, 
            const UMeshData::Handle &meshOutput, const URigData::Handle &rigOutput)
        {
            // for each bone
            for (uint i = 0, n = mesh->mNumBones; i < n; ++i)
            {
                auto bone = mesh->mBones[ i ];

                // get it's name
                auto name = std::string( bone->mName.C_Str( ) );

                // find that name in the hierarchy
                auto boneNode = node->FindNode( name.c_str( ) );

                UAssert( boneNode == nullptr, "Error: Invalid bone!" );

                // add that bone, and all parents, (if not already present) 
                // to my rig (look up in hash map of names in rig data)
            }

            // // set that bones vqs if not already set (scale = transform basis, position = transform origin, q = transform unitX vector)
            // // save the offset matrix
            // // set the weights of the mesh vertices (bone id + weight)
        }

        static void importSceneAnimations(const aiScene *scene, Content3D &content)
        {
            // TODO: import dis shit
            for (uint i = 0; i < scene->mNumAnimations; ++i)
            {
                auto *aiAnim = scene->mAnimations[ i ];
                auto *anim = static_cast<UAnimationData*>( content.animations[ i ].get( ) );

                anim->name = aiAnim->mName.C_Str( );
            }
        }
    }
}
