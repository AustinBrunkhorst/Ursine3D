/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** UModelReader.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "UModelReader.h"

#include "UModelData.h"
#include "UMeshReader.h"

namespace ursine
{
    namespace resources
    {
        // TODO: [J] Remove the "model" concept all together?
        // I.E make everything run off of meshes, and the "Model"
        // generated object would just be a hierarchy of entities

        // TODO: [J] Remove this shit (replace with my own built data)
        struct MeshVertex
        {
            pseudodx::XMFLOAT3	pos;
            pseudodx::XMFLOAT3	normal;
            //pseudodx::XMFLOAT3	binormal;
            pseudodx::XMFLOAT3	tangent;
            pseudodx::XMFLOAT2	uv;
            pseudodx::XMUINT4	ctrlIndices;
            pseudodx::XMFLOAT4	ctrlBlendWeights;

            MeshVertex()
            {
                pos = pseudodx::XMFLOAT3(0.f, 0.f, 0.f);
                normal = pseudodx::XMFLOAT3(0.f, 0.f, 0.f);
                //binormal = pseudodx::XMFLOAT3(0.f, 0.f, 0.f);
                tangent = pseudodx::XMFLOAT3(0.f, 0.f, 0.f);
                uv = pseudodx::XMFLOAT2(0.f, 0.f);
                ctrlIndices = pseudodx::XMUINT4(0, 0, 0, 0);
                ctrlBlendWeights = pseudodx::XMFLOAT4(0.f, 0.f, 0.f, 0.f);
            }

            bool operator==(const MeshVertex& rhs)
            {
                if ((pos == rhs.pos)
                    && (normal == rhs.normal)
                    //&& (binormal == rhs.binormal)
                    && (tangent == rhs.tangent)
                    && (uv == rhs.uv)
                    && (ctrlIndices == rhs.ctrlIndices)
                    && (ctrlBlendWeights == rhs.ctrlBlendWeights))
                    return true;

                return false;
            }

            bool IsValidWeights()
            {
                if ((0.0f == ctrlBlendWeights.x)
                    && (0.0f == ctrlBlendWeights.y)
                    && (0.0f == ctrlBlendWeights.z)
                    && (0.0f == ctrlBlendWeights.w))
                    return false;

                return true;
            }

            bool IsValidControls()
            {
                if ((0 == ctrlIndices.x)
                    && (0 == ctrlIndices.y)
                    && (0 == ctrlIndices.z)
                    && (0 == ctrlIndices.w))
                    return false;

                return true;
            }

            bool CheckSum()
            {
                float sum = ctrlBlendWeights.x + ctrlBlendWeights.y + ctrlBlendWeights.z + ctrlBlendWeights.w;

                if (sum > 1.0f)
                    return false;

                if (!graphics::IsEqualEpsilon(1.0f, sum))
                    return false;

                return true;
            }
        };

        // TODO: [J] Remove this shit
        UModelData::Handle UModelReader::ReadJDL(ResourceReader &input)
        {
            auto output = std::make_shared<UModelData>();

            std::string name;
            uint meshCount, materialCount, boneCount, meshLvlCount, rigLvlCount;

            input.Read( name );

            input.Read( meshCount );
            input.Read( materialCount );
            input.Read( boneCount );
            input.Read( meshLvlCount );
            input.Read( rigLvlCount );

            // read in meshes
            for (uint i = 0; i < meshCount; ++i)
            {
                std::string meshName;
                uint vtxCount, idxCount, mtrlCount, mtrlIdxCount;
                std::vector<MeshVertex> verts;
                auto mesh = std::make_shared<UMeshData>( );

                input.Read( name );
                input.Read( vtxCount );
                input.Read( idxCount );
                input.Read( mtrlCount );
                input.Read( mtrlIdxCount );

                mesh->SetName( name );
                mesh->verts.resize( vtxCount );
                mesh->normals.resize( vtxCount );
                mesh->tangents.resize( vtxCount );

                verts.resize( vtxCount );

                for (uint j = 0; j < vtxCount; ++j)
                {
                    input.Read( verts[ j ] );

                    auto &vert = verts[ j ];

                    mesh->verts[ j ].Set(
                        vert.pos.x, vert.pos.y, vert.pos.z
                    );
                    mesh->normals[ j ].Set(
                        vert.normal.x, vert.normal.y, vert.normal.z
                    );
                    mesh->tangents[ j ].Set(
                        vert.tangent.x, vert.tangent.y, vert.tangent.z
                    );
                }

                mesh->indices.resize( idxCount );

                for (uint j = 0; j < vtxCount; ++j)
                {
                    input.Read( mesh->indices[ j ] );
                }

                output->AddMesh( mesh );
            }

            // add it to the graphics manager
            output->m_id = GetCoreSystem( graphics::GfxAPI )->ResourceMgr.CreateModel( output );

            return output;

            /*unsigned i = 0;

            mMeshInfoVec.resize( mmeshCount );

            for (i = 0; i < mmeshCount; ++i)
                mMeshInfoVec[ i ].Read( input );*/

            /*mMtrlInfoVec.resize( mmaterialCount );

            for (i = 0; i < mmaterialCount; ++i)
                mMtrlInfoVec[ i ].Read( input );

            mBoneInfoVec.resize( mboneCount );

            for (i = 0; i < mboneCount; ++i)
                mBoneInfoVec[ i ].Read( input );

            mMeshLvVec.resize( mmeshlvlCount );

            for (i = 0; i < mmeshlvlCount; ++i)
            {
                auto &mesh = mMeshLvVec[ i ];

                input.Read( mesh.meshName );

                input.Read( mesh.meshTM );
                input.Read( mesh.mParentIndex );
            }
                    
            mRigLvVec.resize( mriglvlCount );

            for (i = 0; i < mriglvlCount; ++i)
            {
                auto &rig = mRigLvVec[ i ];

                input.Read( rig.boneName );

                input.Read( rig.mParentIndex );
            }*/
        }

        UModelReader::UModelReader(void)
        {
        }

        ResourceData::Handle UModelReader::Read(ResourceReader &input)
        {
            auto output = std::make_shared<UModelData>( );
            size_t numMeshes = 0;
            UMeshReader reader;

            input.Read<size_t>(numMeshes);

            while (numMeshes > 0)
            {
                auto mesh = reader.Read( input );

                output->AddMesh( mesh );

                --numMeshes;
            }

            // add it to the graphics manager
            output->m_id = GetCoreSystem( graphics::GfxAPI )->ResourceMgr.CreateModel( output );

            return output;
        }
    }
}
