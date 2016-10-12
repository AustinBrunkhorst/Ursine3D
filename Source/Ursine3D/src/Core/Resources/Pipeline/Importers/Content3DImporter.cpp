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
    // required to get around assimp's fuckery
    // map[bone_name] = vector<vertex, weight>
    typedef std::unordered_map<std::string, std::vector<std::pair<unsigned, float> > > BoneWeightMap;
    typedef std::vector<BoneWeightMap> BoneWeightMaps;

    struct Content3D
    {
        ursine::resources::UModelData::Handle model;
        ursine::resources::URigData::Handle rig;
        std::vector<ursine::resources::UAnimationData::Handle> animations;
        BoneWeightMaps boneWeights;
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

        static void importScene(aiScene *scene, Content3D &content);
        static void importSceneNode(aiScene *scene, aiNode *node, Content3D &content);
        static void importSceneNodeMeshes(aiScene *scene, aiNode *node, Content3D &content);

        static void importMeshVertices(aiMesh *mesh, UMeshData::Handle &output);
        static void importMeshIndices(aiMesh *mesh, UMeshData::Handle &output);

        static void importMeshBonesAndWeights(aiScene *scene, aiNode *node, aiMesh *mesh,
                                              URigData::Handle &rigOutput, BoneWeightMap &weights);
        static void importBones(aiScene *scene, aiBone *bone, aiNode *boneNode, URigData::Handle &output);
        static void importBonesAndParents(aiScene *scene, aiBone *bone, aiNode *boneNode, URigData::Handle &output,
                                          std::vector<URigData::Bone> &bones, uint &insertionIndex);

        static aiBone *getBoneForNode(aiScene *scene, aiNode *node);

        static void importSceneAnimations(aiScene *scene, Content3D &content);

        static void assignSkinnedMeshWeights(Content3D &content);

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

            aiScene *scene = const_cast<aiScene*>( importer.ReadFile( fileName.c_str( ), flags ) );

            UAssertCatchable(
                scene != nullptr || !(scene->mFlags | AI_SCENE_FLAGS_INCOMPLETE),
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

                // Set the rig name
                output.rig->name = rootName.filename( ).string( );

                ResourceWriter writer( rigPath );

                output.rig->Write( writer );

                context.AllocateGeneratedResource( rigPath );
            }

            for (auto &animHandle : output.animations)
            {
                auto *anim = animHandle.get( );

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

        static void importScene(aiScene *scene, Content3D &content)
        {
            if (scene->HasMeshes( ))
            {
                content.model = std::make_shared<UModelData>( );

                // check for bones
                for (uint i = 0, n = scene->mNumMeshes; i < n; ++i)
                {
                    auto &mesh = scene->mMeshes[ i ];

                    if (mesh->mNumBones > 0)
                    {
                        content.rig = std::make_shared<URigData>( );
                        break;
                    }
                }
            }

            if (scene->HasAnimations( ))
            {
                for (uint i = 0, n = scene->mNumAnimations; i < n; ++i)
                    content.animations.emplace_back( std::make_shared<UAnimationData>( ) );
            }

            // iterate through all scene nodes and add their objects to the u3dcontent data
            importSceneNode( scene, scene->mRootNode, content );

            if (content.rig)
            {
                assignSkinnedMeshWeights( content );
            }

            if (scene->HasAnimations( ))
            {
                importSceneAnimations( scene, content );
            }
        }

        static void importSceneNode(aiScene *scene, aiNode *node, Content3D &content)
        {
            if (!node)
                return;

            importSceneNodeMeshes( scene, node, content );

            for (uint i = 0, n = node->mNumChildren; i < n; ++i)
            {
                importSceneNode( scene, node->mChildren[ i ], content );
            }
        }

        static void importSceneNodeMeshes(aiScene *scene, aiNode *node, Content3D &content)
        {
            if (node->mNumMeshes == 0)
                return;

            for (uint i = 0, n = node->mNumMeshes; i < n; ++i)
            {
                auto index = node->mMeshes[ i ];
                auto mesh = scene->mMeshes[ index ];
                auto newMesh = std::make_shared<UMeshData>( );

                newMesh->SetName( node->mName.C_Str( ) );

                importMeshVertices( mesh, newMesh );
                importMeshIndices( mesh, newMesh );

                if (content.rig)
                {
                    // create a new weight map
                    content.boneWeights.push_back( BoneWeightMap( ) );
                    importMeshBonesAndWeights( scene, node, mesh, content.rig, content.boneWeights.back( ) );
                }

                content.model->meshes.emplace_back( newMesh );
            }
        }

        static void importMeshVertices(aiMesh *mesh, UMeshData::Handle &output)
        {
            if (!mesh->HasPositions( ))
                return;

            output->verts.resize( mesh->mNumVertices );

            for (uint i = 0, n = mesh->mNumVertices; i < n; ++i)
            {
                auto &position = mesh->mVertices[ i ];
                auto &normal = mesh->mNormals[ i ];
                auto &tangent = mesh->mTangents[ i ];
                auto &bitangent = mesh->mBitangents[ i ];
                auto &uv = mesh->mTextureCoords[ 0 ][ i ];
                auto &vert = output->verts[ i ];

                vert.position.Set(
                    position.x, position.y, position.z
                );

                vert.normal.Set(
                    normal.x, normal.y, normal.z
                );

                vert.tangent.Set(
                    tangent.x, tangent.y, tangent.z
                );

                vert.bitangent.Set(
                    bitangent.x, bitangent.y, bitangent.z
                );

                vert.uv.Set( uv.x, uv.y );

                vert.boneIndices[ 0 ] =
                vert.boneIndices[ 1 ] =
                vert.boneIndices[ 2 ] =
                vert.boneIndices[ 3 ] = -1;
            }
        }

        static void importMeshIndices(aiMesh *mesh, UMeshData::Handle &output)
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

        static void importMeshBonesAndWeights(aiScene *scene, aiNode *node, aiMesh *mesh,
                                              URigData::Handle &rigOutput, BoneWeightMap &weights)
        {
            // for each bone
            for (uint i = 0, n = mesh->mNumBones; i < n; ++i)
            {
                auto bone = mesh->mBones[ i ];

                // get it's name 
                auto name = std::string( bone->mName.C_Str( ) );

                UAssert( !name.empty( ), "Error: The bone's name should never be empty" );

                // find that name in the hierarchy
                auto boneNode = scene->mRootNode->FindNode( name.c_str( ) );

                UAssert( boneNode != nullptr, "Error: Invalid bone!" );

                importBones( scene, bone, boneNode, rigOutput );

                // set the weights of the mesh vertices (bone id + weight)
                for (uint j = 0, m = bone->mNumWeights; j < m; ++j)
                {
                    auto &weight = bone->mWeights[ j ];

                    // store the weight, bone name, and vert index
                    weights[ name ].push_back( std::make_pair(
                        weight.mVertexId, weight.mWeight
                    ) );
                }
            }
        }

        static void importBones(aiScene *scene, aiBone *bone, aiNode *boneNode, URigData::Handle &output)
        {
            std::vector<URigData::Bone> bones;
            uint insertionIndex = 0;

            importBonesAndParents( scene, bone, boneNode, output, bones, insertionIndex );

            if (bones.size( ) == 0)
                return;

            if (insertionIndex == -1)
                insertionIndex = 0;

            // adjust the parent indices that are going to be shifted
            for (size_t i = insertionIndex + 1, n = output->bones.size( ); i < n; ++i)
            {
                auto &shifted = output->bones[ i ];

                if (shifted.parent < insertionIndex)
                {
                    shifted.parent += static_cast<uint>( bones.size( ) );
                }
            }

            // add all collected bones to the rig
            output->bones.insert( 
                output->bones.begin( ) + insertionIndex, 
                bones.rbegin( ), bones.rend( ) 
            );
        }

        static void importBonesAndParents(aiScene *scene, aiBone *bone, aiNode *boneNode,
                                          URigData::Handle &output, std::vector<URigData::Bone> &bones, uint &insertionIndex)
        {
            auto &boneMap = output->boneMap;
            auto itr = boneMap.find( bone->mName.C_Str( ) );

            // if we've already added this bone
            if (itr != boneMap.end( ))
            {
                insertionIndex = itr->second;
                return;
            }

            // Create and initialize new bone
            URigData::Bone newBone;

            newBone.name = bone->mName.C_Str( );

            // save the offset matrix
            newBone.offset.Set( &bone->mOffsetMatrix.a1 );

            // set that bone's local position/scale/rotation
            aiVector3t<float> scale, position;
            aiQuaterniont<float> rotation;

            boneNode->mTransformation.Decompose( scale, rotation, position );

            newBone.localPosition.Set( position.x, position.y, position.z );
            newBone.localScale.Set( scale.x, scale.y, scale.z );
            newBone.localRotation.Set( rotation.x, rotation.y, rotation.z, rotation.w );

            newBone.numChildren = bones.size( ) == 0 ? 0 : 1;

            // add this new bone to the vector
            // call recursively with this node's parent
            bones.push_back( newBone );

            if (boneNode->mParent == nullptr)
            {
                insertionIndex = -1;
                return;
            }

            // find the bone for this parent
            auto parentBone = getBoneForNode(scene, boneNode->mParent);

            if (parentBone == nullptr)
            {
                insertionIndex = -1;
                return;
            }

            auto size = static_cast<uint>( bones.size( ) );

            importBonesAndParents( scene, parentBone, boneNode->mParent, output, bones, insertionIndex);

            auto newSize = static_cast<uint>( bones.size( ) );

            bones[ size ].parent = insertionIndex + (newSize - size);
        }

        static aiBone *getBoneForNode(aiScene *scene, aiNode *node)
        {
            for (uint i = 0, n = scene->mNumMeshes; i < n; ++i)
            {
                auto &mesh = scene->mMeshes[ i ];

                for (uint j = 0, m = mesh->mNumBones; j < m; ++j)
                {
                    auto &bone = mesh->mBones[ j ];

                    if (bone->mName == node->mName)
                    {
                        return bone;
                    }
                }
            }

            return nullptr;
        }

        static void importSceneAnimations(aiScene *scene, Content3D &content)
        {
            // TODO: import dis shit
            for (uint i = 0; i < scene->mNumAnimations; ++i)
            {
                auto *aiAnim = scene->mAnimations[ i ];
                auto *anim = content.animations[ i ].get( );

                anim->name = aiAnim->mName.C_Str( );

                // TODO: lskjdflksjdf
            }
        }

        static void assignSkinnedMeshWeights(Content3D &content)
        {
            auto &maps = content.boneWeights;
            auto &meshes = content.model->meshes;

            for (size_t i = 0, n = maps.size( ); i < n; ++i)
            {
                auto &map = maps[ i ];
                auto &mesh = meshes[ i ];

                // for every bone in the map
                for (auto &pair : map)
                {
                    auto &boneName = pair.first;
                    auto &boneWeights = pair.second;
                    auto &boneIndex = content.rig->boneMap[ boneName ];

                    for (auto &weightPair : boneWeights)
                    {
                        auto vertIndex = weightPair.first;
                        auto weight = weightPair.second;
                        auto &vert = mesh->verts[ vertIndex ];

                        for (uint slot = 0; slot < 4; ++slot)
                        {
                            if (vert.boneIndices[ slot ] == -1)
                            {
                                vert.boneIndices[ slot ] = boneIndex;
                                vert.boneWeights[ slot ] = weight;
                                slot = 4;
                            }
                        }
                    }
                }
            }
        }
    }
}
